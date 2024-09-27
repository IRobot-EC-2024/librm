/**
 * @file  modules/algorithm/ekf.h
 * @brief EKF姿态解算算法
 */

#ifndef IROBOTEC_MODULES_ALGORITHM_EKF_H
#define IROBOTEC_MODULES_ALGORITHM_EKF_H

#include <array>

#include "irobotec/core/typedefs.h"
#include "irobotec/modules/algorithm/ahrs/ahrs_interface.h"

#include "kalman/LinearizedSystemModel.hpp"

namespace irobotec::modules::algorithm {

class EkfAhrs : public AhrsInterface {
 public:
  explicit EkfAhrs(f32 sample_freq = 1000.0f);
  ~EkfAhrs() override = default;

  void Update(const ImuData9Dof &data) override;
  void Update(const ImuData6Dof &data) override;
  [[nodiscard]] const EulerAngle &euler_angle() const override;
  [[nodiscard]] const Quaternion &quaternion() const override;

 private:
  f32 q0_, q1_, q2_, q3_;
  Quaternion quaternion_;
  EulerAngle euler_ypr_;
  f32 sample_freq_;
};

/**
 * @brief EKF状态向量
 * @tparam T 数据类型
 */
template <typename T>
class State : public Kalman::Vector<T, 6> {
 public:
  KALMAN_VECTOR(State, T, 6)

  //! Q0
  static constexpr size_t Q0 = 0;
  //! Q1
  static constexpr size_t Q1 = 1;
  //! Q2
  static constexpr size_t Q2 = 2;
  //! Q3
  static constexpr size_t Q3 = 3;
  //! WX_BIAS
  static constexpr size_t WX_BIAS = 4;
  //! WY_BIAS
  static constexpr size_t WY_BIAS = 5;

  T q0() const { return (*this)[Q0]; }
  T q1() const { return (*this)[Q1]; }
  T q2() const { return (*this)[Q2]; }
  T q3() const { return (*this)[Q3]; }
  T wx_bias() const { return (*this)[WX_BIAS]; }
  T wy_bias() const { return (*this)[WY_BIAS]; }

  T &q0() { return (*this)[Q0]; }
  T &q1() { return (*this)[Q1]; }
  T &q2() { return (*this)[Q2]; }
  T &q3() { return (*this)[Q3]; }
  T &wx_bias() { return (*this)[WX_BIAS]; }
  T &wy_bias() { return (*this)[WY_BIAS]; }
};

/**
 * @brief EKF控制向量
 *
 * 该EKF无控制向量
 *
 * @tparam T 数据类型
 */
template <typename T>
class Control : public Kalman::Vector<T, 1> {
 public:
  KALMAN_VECTOR(Control, T, 1)
};

template <typename T, template <class> class CovarianceBase = Kalman::StandardBase>
class SystemModel : public Kalman::LinearizedSystemModel<State<T>, Control<T>, CovarianceBase> {
 public:
  //! 状态向量简写
  typedef irobotec::modules::algorithm::State<T> S;
  //! 控制向量简写
  typedef irobotec::modules::algorithm::Control<T> C;

  /**
   * @brief 构造函数
   *
   * 将采样频率和陀螺仪数据传进来，供预测更新使用
   *
   * @param sample_freq 采样频率
   * @param gx 陀螺仪X轴数据
   * @param gy 陀螺仪Y轴数据
   * @param gz 陀螺仪Z轴数据
   */
  SystemModel(const f32 sample_freq, T gx, T gy, T gz) : sample_freq_(sample_freq), gx_(gx), gy_(gy), gz_(gz) {}

  /**
   * @brief 预测更新
   *
   * 该函数用于预测更新状态向量
   *
   * @param x 状态向量
   * @param u 控制向量
   * @returns 预测更新后的状态向量
   */
  S f(const S &x, const C &u) const override {
    //! 预测更新状态向量
    S x_;

    // 中间量
    f32 dt = 1.0f / sample_freq_;
    T gx_real = gx_ - x.wx_bias();
    T gy_real = gy_ - x.wy_bias();
    T gz_real = gz_;  // 大部分时间机体z轴通天，无法用加速度计校准

    // 四元数预测更新
    // -  0 -gx -gy -gz  -     -  q0  -
    // |  gx  0  gz -gy  |  *  |  q1  |  *  0.5 * dt
    // |  gy -gz  0  gx  |     |  q2  |
    // -  gz  gy -gx  0  -     -  q3  -
    x_.q0() += (-x.q1() * gx_real - x.q2() * gy_real - x.q3() * gz_real) * dt * 0.5f;
    x_.q1() += (x.q0() * gx_real + x.q2() * gz_real - x.q3() * gy_real) * dt * 0.5f;
    x_.q2() += (x.q0() * gy_real - x.q1() * gz_real + x.q3() * gx_real) * dt * 0.5f;
    x_.q3() += (x.q0() * gz_real + x.q1() * gy_real - x.q2() * gx_real) * dt * 0.5f;

    // 陀螺仪x，y轴误差预测更新
    x_.wx_bias() = x.wx_bias();
    x_.wy_bias() = x.wy_bias();

    return x_;
  }

 protected:
  /**
   * @brief 更新雅可比矩阵
   *
   * 该函数用于更新雅可比矩阵
   *
   * @param x 状态向量
   * @param u 控制向量
   */
  void updateJacobians(const S &x, const C &u) override {
    // 中间量
    f32 dt = 1.0f / sample_freq_;
    T gx_real = gx_ - x.wx_bias();
    T gy_real = gy_ - x.wy_bias();
    T gz_real = gz_;  // 大部分时间机体z轴通天，无法用加速度计校准

    // F = df/dx
    this->F.setIdentity();  // 现在 F = I

    Kalman::Matrix<T, 4, 4> Fq;
    Fq << 0, -gx_real, -gy_real, -gz_real,  //
        gx_real, 0, gz_real, -gy_real,      //
        gy_real, -gz_real, 0, gx_real,      //
        gz_real, gy_real, -gx_real, 0;      //
    this->F.template block<4, 4>(S::Q0, S::Q0) += Fq * dt * 0.5f;

    Kalman::Matrix<T, 4, 2> Fw;
    Fw << x.q1, x.q2,  //
        -x.q0, x.q3,   //
        -x.q3, -x.q0,  //
        x.q2, -x.q1;   //
    this->F.template block<4, 2>(S::Q0, S::WX_BIAS) += Fw * dt * 0.5f;

    // W = df/dw
    this->W.setIdentity();
  }


 private:
  f32 sample_freq_;
  T gx_, gy_, gz_;
};

}  // namespace irobotec::modules::algorithm

#endif  // IROBOTEC_MODULES_ALGORITHM_EKF_H
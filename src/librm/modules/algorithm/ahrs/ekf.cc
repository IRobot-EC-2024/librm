#include "ekf.h"

#include "librm/modules/algorithm/ahrs/ahrs_interface.h"
#include "librm/modules/algorithm/utils.hpp"

namespace rm::modules::algorithm {

EkfAhrs::EkfAhrs(f32 sample_freq)
    : sample_freq_(sample_freq),
      q0_(1.0f),
      q1_(0.0f),
      q2_(0.0f),
      q3_(0.0f),
      quaternion_{1.0f, 0.0f, 0.0f, 0.0f},
      euler_ypr_{0.0f, 0.0f, 0.0f},
      sys_(sample_freq_) {
  x_.setZero();

  x_.q0() = 1.0f;
  x_.q1() = 0.0f;
  x_.q2() = 0.0f;
  x_.q3() = 0.0f;

  x_.wx_bias() = 0.004f;
  x_.wy_bias() = 0.006f;

  ekf_.init(x_);
}

void EkfAhrs::Update(const rm::modules::algorithm::ImuData9Dof &data) {}

/**
 * @brief       更新数据（无磁力计）
 * @param data  IMU数据
 */
void EkfAhrs::Update(const rm::modules::algorithm::ImuData6Dof &data) {
  sys_.GetGyro(data.gx, data.gy, data.gz);

  auto Q = sys_.getCovariance();

  Q(0, 0) = 10.0f * (1.0f / 1000.0f);
  Q(1, 1) = 10.0f * (1.0f / 1000.0f);
  Q(2, 2) = 10.0f * (1.0f / 1000.0f);
  Q(3, 3) = 10.0f * (1.0f / 1000.0f);
  Q(4, 4) = 0.001f;
  Q(5, 5) = 0.001f;

  sys_.setCovariance(Q);

  auto R = amm_.getCovariance();

  R(0, 0) = 1000000.0f;
  R(1, 1) = 1000000.0f;
  R(2, 2) = 1000000.0f;

  amm_.setCovariance(R);

  x_ = sys_.f(x_, u_);

  auto x_ekf = ekf_.predict(sys_);

  am_ = amm_.h(x_);

  am_.ax() = data.ax; 
  am_.ay() = data.ay;
  am_.az() = data.az;

  am_.normalize();

  x_ekf = ekf_.update(amm_, am_);

  quaternion_.w = ekf_.getState().q0();
  quaternion_.x = ekf_.getState().q1();
  quaternion_.y = ekf_.getState().q2();
  quaternion_.z = ekf_.getState().q3();

  f32 euler_ypr_temp[3], quaternion_temp[4] = {quaternion_.w, quaternion_.x, quaternion_.y, quaternion_.z};
  utils::QuatToEuler(quaternion_temp, euler_ypr_temp);
  euler_ypr_.yaw = euler_ypr_temp[0];
  euler_ypr_.pitch = euler_ypr_temp[1];
  euler_ypr_.roll = euler_ypr_temp[2];
}

/**
 * @return 姿态欧拉角
 */
[[nodiscard]] const EulerAngle &EkfAhrs::euler_angle() const { return euler_ypr_; }

/**
 * @return 姿态四元数
 */
[[nodiscard]] const Quaternion &EkfAhrs::quaternion() const { return quaternion_; }

}  // namespace rm::modules::algorithm
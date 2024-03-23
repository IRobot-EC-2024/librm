
/**
 * @file  modules/pid/c_pid.h
 * @brief PID控制器
 */

#ifndef EC_LIB_MODULES_ALGORITHM_PID_H
#define EC_LIB_MODULES_ALGORITHM_PID_H

#include <memory>

#include "modules/typedefs.h"

namespace irobot_ec::modules::algorithm::PID {

enum class PIDType {
  kPosition,
  kDelta,
};

/**
 * @brief PID控制器
 * @note  该PID控制器支持使用外部提供的微分输入
 *        例如：电机位置闭环控制中，可以使用电机速度作为微分输入
 *        或：姿态控制中，可以使用陀螺仪输出作为微分输入，以减小位置误差和速度误差之间的耦合
 *        要使用外部提供的微分输入，只需在构造函数中多传入一个指向微分输入量的指针即可
 * @warning 外部微分输入的类型必须是fp32，
 *          且必须保证外部微分输入变量的生命周期大于PID控制器对象的生命周期
 * @warning 使用外部提供的微分输入时，请注意调整Kd，以避免微分输入的幅值过大
 */
class PID {
 public:
  PID() = delete;
  PID(PIDType type, fp32 kp, fp32 ki, fp32 kd, fp32 max_out, fp32 max_iout);
  PID(PIDType type, fp32 kp, fp32 ki, fp32 kd, fp32 max_out, fp32 max_iout,
      fp32 *external_diff_input);
  virtual void update(fp32 set, fp32 ref);
  void clear();
  void switchParameter(fp32 kp, fp32 ki, fp32 kd, fp32 max_out, fp32 max_iout);
  [[nodiscard]] fp32 value() const;

 protected:
  fp32 kp_;
  fp32 ki_;
  fp32 kd_;

  fp32 max_out_{};
  fp32 max_iout_{};

  fp32 set_{};
  fp32 fdb_{};

  fp32 out_{};
  fp32 p_out_{};
  fp32 i_out_{};
  fp32 d_out_{};
  fp32 d_buf_[3]{};  // 0: 这次, 1: 上次, 2: 上上次
  fp32 error_[3]{};  // 0: 这次, 1: 上次, 2: 上上次

  fp32 *external_diff_input_;     // 外部提供的微分输入
  bool use_external_diff_input_;  // 是否使用外部提供的微分输入

  PIDType type_;

};  // class PID

/**
 * @brief 带过零点处理的PID控制器，可以用于电机位置闭环控制等情况
 */
class RingPID : public PID {
 public:
  RingPID() = delete;
  RingPID(PIDType type, fp32 kp, fp32 ki, fp32 kd, fp32 max_out, fp32 max_iout,
          fp32 cycle);
  void update(fp32 set, fp32 ref) override;

 protected:
  void handleZeroCrossing();

  fp32 cycle_;

};  // class RingPID

}  // namespace irobot_ec::modules::algorithm::PID

#endif  // EC_LIB_MODULES_ALGORITHM_PID_H

/* EOF */

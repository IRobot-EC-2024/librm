/*
  Copyright (c) 2024 XDU-IRobot

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
*/

/**
 * @file  irobotec/modules/algorithm/pid.h
 * @brief PID控制器
 */

#ifndef IROBOTEC_MODULES_ALGORITHM_PID_H
#define IROBOTEC_MODULES_ALGORITHM_PID_H

#include <memory>

// 如果dsp库可用，就启用dsp库里的pid控制器支持
#include "irobotec/modules/dsp.h"
#include "irobotec/core/typedefs.h"

namespace irobot_ec::modules::algorithm {

enum class PIDType {
  kPosition,
  kDelta,
#if defined(ARM_MATH_DSP)
  kDsp,
#endif
};

/**
 * @brief PID控制器
 * @note  该PID控制器支持使用外部提供的微分输入
 *        例如：电机位置闭环控制中，可以使用电机速度作为微分输入
 *        或：姿态控制中，可以使用陀螺仪输出作为微分输入，以减小位置误差和速度误差之间的耦合
 *        要使用外部提供的微分输入，只需在构造函数中多传入一个指向微分输入量的指针即可
 * @warning 外部微分输入的类型必须是f32，
 *          且必须保证外部微分输入变量的生命周期大于PID控制器对象的生命周期
 * @warning 使用外部提供的微分输入时，请注意调整Kd，以避免微分输入的幅值过大
 * @warning 使用DSP库实现时本控制器不支持积分限幅，即使设置了max_iout参数也不会生效
 */
class PID {
 public:
  PID(PIDType type, f32 kp, f32 ki, f32 kd, f32 max_out, f32 max_iout);
  PID(PIDType type, f32 kp, f32 ki, f32 kd, f32 max_out, f32 max_iout, f32 *external_diff_input);
  virtual void update(f32 set, f32 ref);
  void clear();
  void switchParameter(f32 kp, f32 ki, f32 kd, f32 max_out, f32 max_iout);
  [[nodiscard]] f32 value() const;

 protected:
  f32 kp_;
  f32 ki_;
  f32 kd_;

  f32 max_out_{};
  f32 max_iout_{};

  f32 set_{};
  f32 fdb_{};

  f32 out_{};
  f32 p_out_{};
  f32 i_out_{};
  f32 d_out_{};
  f32 d_buf_[3]{};  // 0: 这次, 1: 上次, 2: 上上次
  f32 error_[3]{};  // 0: 这次, 1: 上次, 2: 上上次

#if defined(ARM_MATH_DSP)
  arm_pid_instance_f32 dsp_pid_;
#endif
  f32 *external_diff_input_;      // 外部提供的微分输入
  bool use_external_diff_input_;  // 是否使用外部提供的微分输入

  PIDType type_;
};  // class PID

/**
 * @brief 带过零点处理的PID控制器，可以用于电机位置闭环控制等情况
 */
class RingPID : public PID {
 public:
  RingPID() = delete;
  RingPID(PIDType type, f32 kp, f32 ki, f32 kd, f32 max_out, f32 max_iout, f32 cycle);
  void update(f32 set, f32 ref) override;

 protected:
  void handleZeroCrossing();

  f32 cycle_;
};  // class RingPID

}  // namespace irobot_ec::modules::algorithm

#endif  // IROBOTEC_MODULES_ALGORITHM_PID_H

/* EOF */

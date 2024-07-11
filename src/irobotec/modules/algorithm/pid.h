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
#include <tuple>

#include "irobotec/core/typedefs.h"

namespace irobot_ec::modules::algorithm {

enum class PIDType {
  kPosition,
  kDelta,
};

/**
 * @brief PID控制器
 */
class PID {
 public:
  PID(PIDType type, f32 kp, f32 ki, f32 kd, f32 max_out, f32 max_iout);
  virtual void Update(f32 set, f32 ref);
  virtual void Update(f32 set, f32 ref, f32 external_diff);
  void Clear();
  [[nodiscard]] f32 value() const;

 public:
  f32 kp_{};
  f32 ki_{};
  f32 kd_{};

  f32 max_out_{};
  f32 max_iout_{};

 protected:
  f32 set_{};
  f32 fdb_{};

  f32 out_{};
  f32 p_out_{};
  f32 i_out_{};
  f32 d_out_{};
  f32 d_buf_[3]{};  // 0: 这次, 1: 上次, 2: 上上次
  f32 error_[3]{};  // 0: 这次, 1: 上次, 2: 上上次

  PIDType type_;
};  // class PID

/**
 * @brief 带过零点处理的PID控制器，可以用于电机位置闭环控制等情况
 */
class RingPID : public PID {
 public:
  RingPID() = delete;
  RingPID(PIDType type, f32 kp, f32 ki, f32 kd, f32 max_out, f32 max_iout, f32 cycle);
  void Update(f32 set, f32 ref) override;
  void Update(f32 set, f32 ref, f32 external_diff) override;

 protected:
  void HandleZeroCrossing();

  f32 cycle_;
};  // class RingPID

}  // namespace irobot_ec::modules::algorithm

#endif  // IROBOTEC_MODULES_ALGORITHM_PID_H

/* EOF */

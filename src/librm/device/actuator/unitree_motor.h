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
 * @file  librm/device/actuator/unitree_motor.h
 * @brief 宇树电机类库
 * @todo  未完成
 */

#ifndef LIBRM_DEVICE_ACTUATOR_UNITREE_MOTOR_H
#define LIBRM_DEVICE_ACTUATOR_UNITREE_MOTOR_H

#include "librm/hal/serial.h"
#include "librm/core/typedefs.h"

namespace rm::device {

enum class UnitreeMotorMode {
  kLock = 0x00,
  kFOC = 0x01,
  kCalibration = 0x02,
  kReserved1 = 0x03,
  kReserved2 = 0x04,
  kReserved3 = 0x05,
  kReserved4 = 0x06,
  kReserved5 = 0x07,
};

class UnitreeMotorBase {
 public:
  UnitreeMotorBase() = delete;
  ~UnitreeMotorBase() = default;

 protected:
  hal::SerialInterface *serial_;
  u8 tx_buffer_[17];
  u16 id_{};

  /** MOTOR FEEDBACK DATA **/
  UnitreeMotorMode mode_{};
  i16 tau_{};       // Divide by 256 to get the real value(N*m)
  i16 velocity_{};  // Divide by 1000 to get the real value(rad/s)
  /*************************/
};

}  // namespace rm::device

#endif

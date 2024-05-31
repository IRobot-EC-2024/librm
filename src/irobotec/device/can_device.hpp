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
 * @file  irobotec/device/can_device.hpp
 * @brief CAN设备抽象类
 */

#ifndef IROBOTEC_CAN_DEVICE_HPP
#define IROBOTEC_CAN_DEVICE_HPP

#include "irobotec/hal/hal.h"
#if defined(HAL_CAN_MODULE_ENABLED) || defined(HAL_FDCAN_MODULE_ENABLED)

#include "irobotec/hal/can_interface.h"

namespace irobot_ec::device {

/**
 * @brief CAN设备的基类
 */
class CanDeviceBase {
 public:
  virtual ~CanDeviceBase() = default;

  template <typename... IdList>
  explicit CanDeviceBase(hal::CanInterface &can, IdList... rx_std_ids);

  // 禁止拷贝构造
  CanDeviceBase(const CanDeviceBase &) = delete;
  CanDeviceBase &operator=(const CanDeviceBase &) = delete;

  virtual void RxCallback(const hal::CanMsg *msg) = 0;

 protected:
  hal::CanInterface *can_;
};

/**
 * @param can        CAN外设对象
 * @param rx_std_ids 这个设备的rx消息标准帧id列表
 */
template <typename... IdList>
CanDeviceBase::CanDeviceBase(hal::CanInterface &can, IdList... rx_std_ids) : can_(&can) {
  (can.RegisterDevice(*this, rx_std_ids), ...);
}

}  // namespace irobot_ec::device

#endif

#endif  // IROBOTEC_CAN_DEVICE_HPP

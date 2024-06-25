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
 * @file  irobotec/hal/can.h
 * @brief 根据平台宏定义决定Can的具体实现，并且在can_interface.h里提供一个接口类CanInterface实现多态
 */

#ifndef IROBOTEC_HAL_CAN_H
#define IROBOTEC_HAL_CAN_H

#include "irobotec/hal/stm32/bxcan.h"
#include "irobotec/hal/stm32/fdcan.h"
#include "irobotec/hal/linux/socketcan.h"

namespace irobot_ec::hal {
#if defined(IROBOTEC_PLATFORM_STM32)
#if defined(HAL_CAN_MODULE_ENABLED)
using Can = stm32::BxCan;
#elif defined(HAL_FDCAN_MODULE_ENABLED)
using Can = stm32::FdCan;  // TODO: 实现FdCan类
#endif
#elif defined(IROBOTEC_PLATFORM_LINUX)
using Can = linux_::SocketCan;
#endif
}  // namespace irobot_ec::hal

#endif  // IROBOTEC_HAL_CAN_H

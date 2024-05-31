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
 * @file  irobotec/hal/stm32/fdcan.h
 * @brief fdCAN类库
 * @todo  未完成
 */

#ifndef IROBOTEC_HAL_STM32_FDCAN_H
#define IROBOTEC_HAL_STM32_FDCAN_H

#include "irobotec/hal/hal.h"
#if defined(HAL_FDCAN_MODULE_ENABLED)

#include "irobotec/hal/can_interface.h"
#include "irobotec/device/can_device.hpp"

namespace irobot_ec::hal {

class FdCan : public CanBase {};

}  // namespace irobot_ec::hal

#endif

#endif  // IROBOTEC_HAL_STM32_FDCAN_H
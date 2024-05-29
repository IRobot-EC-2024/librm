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
 * @file  irobotec.hpp
 * @brief irobotEC库的主头文件
 */

#ifndef IROBOTEC_H
#define IROBOTEC_H

/******** CORE ********/
#include "core/typedefs.h"
#include "core/exception.h"
#include "core/time.h"
/****************/

/******** HAL WRAPPER ********/
#include "hal/hal.h"
#include "hal/can.h"
#include "hal/uart.h"
#include "hal/i2c_device.h"
#include "hal/spi_device.h"
/****************/

/******** DEVICE ********/
#include "device/device.h"
#include "device/can_device.hpp"
#include "device/actuator/dji_motor.hpp"
#include "device/actuator/unitree_motor.h"
#include "device/remote/dr16.h"
#include "device/sensor/bmi088/bmi088.h"
#include "device/sensor/ist8310/ist8310.h"
#include "device/supercap/supercap.h"
/****************/

/******** MISC MODULES ********/
/****************/

#endif  // IROBOTEC_H

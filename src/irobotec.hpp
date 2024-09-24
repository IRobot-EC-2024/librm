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
#include "irobotec/core/thread_pool.hpp"
#include "irobotec/core/typedefs.h"
#include "irobotec/core/exception.h"
#include "irobotec/core/time.hpp"
/****************/

/******** HAL WRAPPER ********/
#include "irobotec/hal/can.h"
#include "irobotec/hal/gpio.h"
#include "irobotec/hal/serial.h"
#include "irobotec/hal/spi.h"
/****************/

/******** DEVICE ********/
#include "irobotec/device/device.h"
#include "irobotec/device/can_device.hpp"
#include "irobotec/device/actuator/dji_motor.hpp"
#include "irobotec/device/actuator/dm_motor.hpp"
#include "irobotec/device/actuator/unitree_motor.h"
#include "irobotec/device/remote/dr16.h"
#include "irobotec/device/sensor/bmi088.h"
#include "irobotec/device/sensor/ist8310.h"
#include "irobotec/device/supercap/supercap.h"
/****************/

/******** MISC MODULES ********/
#include "irobotec/modules/algorithm/ahrs/mahony.h"
#include "irobotec/modules/algorithm/chassis_fkik.h"
#include "irobotec/modules/algorithm/crc.h"
#include "irobotec/modules/algorithm/pid.h"
#include "irobotec/modules/algorithm/utils.hpp"
#include "irobotec/modules/vofa_plotter.hpp"
/****************/

#endif  // IROBOTEC_H

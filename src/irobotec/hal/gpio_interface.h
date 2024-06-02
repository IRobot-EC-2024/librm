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
 * @file    irobotec/hal/gpio_interface.h
 * @brief   GPIO的接口类
 */

#ifndef IROBOTEC_HAL_GPIO_INTERFACE_H
#define IROBOTEC_HAL_GPIO_INTERFACE_H

namespace irobot_ec::hal {

/**
 * @brief GPIO引脚接口类
 */
class PinInterface {
 public:
  virtual ~PinInterface() = default;

  virtual void Write(bool state) = 0;

  [[nodiscard]] virtual bool Read() const = 0;
};

}  // namespace irobot_ec::hal

#endif  // IROBOTEC_HAL_GPIO_INTERFACE_H

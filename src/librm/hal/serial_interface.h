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
 * @file    librm/hal/serial_interface.h
 * @brief   串口的接口类
 */

#ifndef LIBRM_HAL_UART_INTERFACE_H
#define LIBRM_HAL_UART_INTERFACE_H

#include <functional>
#include <vector>

#include "librm/core/typedefs.h"

namespace rm::hal {

/**
 * @brief 串口配置结构体
 */
struct SerialConfig {
  usize baud_rate;
  enum class Parity { None, Odd, Even } parity{Parity::None};
  enum class DataBits { Five = 5, Six, Seven, Eight } data_bits{DataBits::Eight};
  enum class StopBits { One = 1, Two } stop_bits{StopBits::One};
};

/**
 * @brief 串口接收完成回调函数类型，传入的参数分别为接收到的数据和数据长度
 */
using SerialRxCallbackFunction = std::function<void(const std::vector<u8> &, u16)>;

/**
 * @brief 串口接口类
 */
class SerialInterface {
 public:
  virtual ~SerialInterface() = default;

  /**
   * @brief 初始化串口
   */
  virtual void Begin() = 0;

  /**
   * @brief 发送数据
   * @param data 数据指针
   * @param size 数据长度
   */
  virtual void Write(const u8 *data, usize size) = 0;

  /**
   * @brief 绑定接收完成回调函数
   * @param callback 回调函数
   */
  virtual void AttachRxCallback(SerialRxCallbackFunction &callback) = 0;

  /**
   * @brief 获取接收缓冲区
   * @return 接收缓冲区
   */
  [[nodiscard]] virtual const std::vector<u8> &rx_buffer() const = 0;
};

}  // namespace rm::hal

#endif  // LIBRM_HAL_UART_INTERFACE_H

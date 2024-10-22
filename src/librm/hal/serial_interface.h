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

#include <chrono>
#include <functional>
#include <vector>
#include <string>

#include "librm/core/typedefs.h"

namespace rm::hal {

/**
 * @brief 串口配置结构体
 * @note  默认配置：波特率 115200，无校验位，数据位 8 位，停止位 1 位，超时时间无限
 */
struct SerialConfig {
  usize baud_rate{115200};
  enum class Parity { None, Odd, Even } parity{Parity::None};
  enum class DataBits { Five = 5, Six, Seven, Eight } data_bits{DataBits::Eight};
  enum class StopBits { One = 1, Two } stop_bits{StopBits::One};
  enum class FlowControl { None, Software, Hardware } flow_control{FlowControl::None};
  std::chrono::milliseconds timeout{std::chrono::milliseconds::max()};
};

/**
 * @brief 串口接收完成回调函数
 */
using SerialRxCallbackFunction = std::function<void(const std::vector<u8> &)>;

/**
 * @brief 串口接口类
 */
class SerialInterface {
 public:
  virtual ~SerialInterface() = default;

  /**
   * @brief 初始化串口，开始接收
   */
  virtual void Begin() = 0;

  /**
   * @brief 发送数据
   * @param data 数据指针
   * @param size 数据长度
   */
  virtual void Write(const u8 *data, usize size) = 0;

  /**
   * @brief 发送数据
   * @note  这个方法可以不用实现，因为默认实现会调用Write(const u8 *, usize)
   * @param data 数据
   */
  inline void Write(const std::string_view data) { Write(reinterpret_cast<const u8 *>(data.data()), data.size()); }

  /**
   * @brief 发送数据
   * @note  这个方法可以不用实现，因为默认实现会调用Write(const u8 *, usize)
   * @param data 数据
   */
  inline void Write(const std::vector<u8> &data) { Write(data.data(), data.size()); }

  /**
   * @brief 绑定接收完成回调函数
   * @param callback 回调函数
   */
  virtual void AttachRxCallback(SerialRxCallbackFunction &callback) = 0;
};

}  // namespace rm::hal

#endif  // LIBRM_HAL_UART_INTERFACE_H

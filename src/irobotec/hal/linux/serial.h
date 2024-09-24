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
 * @file  irobotec/hal/linux/serial.h
 * @brief 串口类库
 */

#ifndef IROBOTEC_HAL_LINUX_SERIAL_H
#define IROBOTEC_HAL_LINUX_SERIAL_H

#include <string>
#include <chrono>
#include <mutex>

#include "serial/serial.h"

#include "irobotec/hal/serial_interface.h"
#include "irobotec/core/thread_pool.hpp"

namespace irobotec::hal::linux_ {

class Serial : public hal::SerialInterface {
 public:
  Serial() = delete;
  Serial(const char *dev, usize baud, usize rx_buffer_size, std::chrono::milliseconds timeout);
  ~Serial() override;

  void Begin() override;
  void Write(const u8 *data, usize size) override;
  void AttachRxCallback(SerialRxCallbackFunction &callback) override;
  [[nodiscard]] const std::vector<u8> &rx_buffer() const override;

 private:
  void RecvThread();

  SerialRxCallbackFunction *rx_callback_{nullptr};
  serial::Serial serial_{};
  std::string dev_{};

  std::unique_ptr<core::ThreadPool> thread_pool_{};  // 线程池，用于异步调用回调函数和创建轮询线程
  std::mutex callback_mutex_{};  // 回调函数由用户编写，并不能保证线程安全，因此调用时需要加锁

  // 双缓冲，防止在回调函数中读取数据时被覆盖
  std::vector<u8> rx_buf_[2];
  bool buffer_selector_{false};

  /**
   * @brief 最大线程数
   * @note  用于创建线程池
   */
  static constexpr usize kMaxThreads = 20;
};

}  // namespace irobotec::hal::linux_

#endif
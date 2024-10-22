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
 * @file  librm/hal/linux/serial.h
 * @brief 串口类库
 */

#ifndef LIBRM_HAL_LINUX_SERIAL_H
#define LIBRM_HAL_LINUX_SERIAL_H

#include <mutex>

#include "librm/hal/serial_interface.h"
#include "librm/core/thread_pool.hpp"

namespace rm::hal::linux_ {

class Serial : public hal::SerialInterface {
 public:
  Serial() = delete;
  ~Serial() override;
  Serial(const std::string_view dev, SerialConfig config = SerialConfig{});

  void Begin() override;
  void Write(const u8 *data, usize size) override;
  void AttachRxCallback(SerialRxCallbackFunction &callback) override;

 private:
  void RecvThread();

  int serial_fd_{};
  SerialRxCallbackFunction *rx_callback_{nullptr};
  SerialConfig config_{};
  std::string dev_{};
  std::thread recv_thread_{};
  core::ThreadPool thread_pool_{kMaxThreads};  // 线程池，用于异步调用回调函数和创建轮询线程
  std::mutex callback_mutex_{};  // 回调函数由用户编写，并不能保证线程安全，因此调用时需要加锁
  std::mutex write_mutex_{};     // 写入数据时需要加锁

  // 双缓冲，防止在回调函数中读取数据时被覆盖
  std::vector<u8> rx_buf_[2];
  bool buffer_selector_{false};

  /**
   * @brief 最大线程数
   * @note  用于创建线程池
   */
  static constexpr usize kMaxThreads = 20;
};

}  // namespace rm::hal::linux_

#endif
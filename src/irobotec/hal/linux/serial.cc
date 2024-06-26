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
 * @file  irobotec/hal/linux/serial.cc
 * @brief 串口类库
 */

#include "serial.h"

#include <functional>

namespace irobot_ec::hal::linux_ {

Serial::Serial(const char *dev, usize baud, usize rx_buffer_size, std::chrono::milliseconds timeout)
    : dev_(dev),
      serial_(dev, baud, serial::Timeout::simpleTimeout(timeout.count())),
      rx_buf_{std::vector<u8>(rx_buffer_size), std::vector<u8>(rx_buffer_size)},
      thread_pool_(std::make_unique<core::ThreadPool>(Serial::kMaxThreads)) {
  if (!this->serial_.isOpen()) {
    throw std::runtime_error("Failed to open serial port");
  }
}

Serial::~Serial() { this->serial_.close(); }

void Serial::Begin() { this->thread_pool_->enqueue(std::bind(&Serial::RecvThread, this)); }

void Serial::Write(const u8 *data, usize size) {
  this->serial_.write(data, size);
  this->serial_.flush();
}

void Serial::AttachRxCallback(UartCallbackFunction &callback) { this->rx_callback_ = &callback; }

[[nodiscard]] const std::vector<u8> &Serial::rx_buffer() const { return this->rx_buf_[this->buffer_selector_]; }

void Serial::RecvThread() {
  for (;;) {
    auto bytes_read =
        this->serial_.read(this->rx_buf_[this->buffer_selector_], this->rx_buf_[this->buffer_selector_].size());
    if (bytes_read == 0) {
      continue;
    }
    this->buffer_selector_ = !this->buffer_selector_;  // 切换缓冲区

    // 异步调用接收完成回调函数
    if (this->rx_callback_ != nullptr) {
      this->thread_pool_->enqueue([this, bytes_read]() {
        std::lock_guard<std::mutex> lock(this->callback_mutex_);
        (*this->rx_callback_)(this->rx_buf_[this->buffer_selector_], bytes_read);
      });
    }
  }
}

}  // namespace irobot_ec::hal::linux_
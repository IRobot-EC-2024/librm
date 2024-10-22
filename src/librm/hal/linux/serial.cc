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
 * @file  librm/hal/linux/serial.cc
 * @brief 串口类库
 */

#include "serial.h"

#include <fcntl.h>
#include <unistd.h>
#include <mutex>

#define termios asmtermios
#include <asm/termios.h>
#undef termios

#include <termios.h>

#include "librm/hal/serial_interface.h"
#include "librm/core/exception.h"

extern "C" {
extern int ioctl(int __fd, unsigned long int __request, ...) throw();
}

namespace rm::hal::linux_ {

Serial::~Serial() { ::close(serial_fd_); }

Serial::Serial(std::string_view dev, SerialConfig config) {
  this->serial_fd_ = ::open(dev.data(), O_RDWR | O_NOCTTY);
  if (this->serial_fd_ == -1) {
    core::exception::ThrowException(core::exception::Exception::kException);
  }

  struct termios2 options {};

  if (::ioctl(this->serial_fd_, TCGETS2, &options) == -1) {
    core::exception::ThrowException(core::exception::Exception::kException);
  }

  options.c_cflag &= ~CBAUD;
  options.c_cflag |= BOTHER;
  options.c_ispeed = config.baud_rate;
  options.c_ospeed = config.baud_rate;

  options.c_cflag &= ~CSIZE;
  switch (config.data_bits) {
    case SerialConfig::DataBits::Five:
      options.c_cflag |= CS5;
      break;
    case SerialConfig::DataBits::Six:
      options.c_cflag |= CS6;
      break;
    case SerialConfig::DataBits::Seven:
      options.c_cflag |= CS7;
      break;
    case SerialConfig::DataBits::Eight:
      options.c_cflag |= CS8;
      break;
  }

  switch (config.parity) {
    case SerialConfig::Parity::None:
      options.c_cflag &= ~PARENB;
      break;
    case SerialConfig::Parity::Odd:
      options.c_cflag |= PARENB;
      options.c_cflag |= PARODD;
      break;
    case SerialConfig::Parity::Even:
      options.c_cflag |= PARENB;
      options.c_cflag &= ~PARODD;
      break;
  }

  switch (config.stop_bits) {
    case SerialConfig::StopBits::One:
      options.c_cflag &= ~CSTOPB;
      break;
    case SerialConfig::StopBits::Two:
      options.c_cflag |= CSTOPB;
      break;
  }

  if (config.flow_control == SerialConfig::FlowControl::Software) {
    options.c_iflag |= IXON | IXOFF | IXANY;
  } else if (config.flow_control == SerialConfig::FlowControl::Hardware) {
    options.c_cflag |= CRTSCTS;
  }

  options.c_cc[VMIN] = 0;
  options.c_cc[VTIME] = 0;

  if (::ioctl(this->serial_fd_, TCSETS2, &options) == -1) {
    core::exception::ThrowException(core::exception::Exception::kException);
  }

  ::ioctl(this->serial_fd_, TCSETS2, &options);

  ::tcflush(this->serial_fd_, TCIOFLUSH);  // flush the buffer
}

void Serial::Begin() { recv_thread_ = std::thread(&Serial::RecvThread, this); }

void Serial::Write(const u8 *data, usize size) {
  std::lock_guard<std::mutex> lock(write_mutex_);
  ::write(serial_fd_, data, size);
}

void Serial::AttachRxCallback(SerialRxCallbackFunction &callback) { this->rx_callback_ = &callback; }

void Serial::RecvThread() {
  for (;;) {
    // 如果没有绑定回调函数，就什么也不做
    if (this->rx_callback_ == nullptr) {
      continue;
    }

    rx_buf_[buffer_selector_].clear();
    ::read(serial_fd_, rx_buf_[buffer_selector_].data(), rx_buf_[buffer_selector_].size());
    this->buffer_selector_ = !this->buffer_selector_;  // 切换缓冲区

    // 异步调用回调函数，通知数据接收完成
    this->thread_pool_.enqueue([this]() {
      std::lock_guard<std::mutex> lock(this->callback_mutex_);
      (*this->rx_callback_)(this->rx_buf_[this->buffer_selector_]);
    });
  }
}

}  // namespace rm::hal::linux_

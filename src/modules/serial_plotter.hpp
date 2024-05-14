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
 * @file  modules/serial_plotter.hpp
 * @brief 串口绘图器
 */

#ifndef EC_LIB_MODULES_SERIAL_PLOTTER_HPP
#define EC_LIB_MODULES_SERIAL_PLOTTER_HPP

#include <iomanip>
#include <list>
#include <sstream>
#include <variant>

#include "modules/typedefs.h"

namespace irobot_ec::modules {

using VariableVariant = std::variant<i8 *, i16 *, i32 *, i64 *, u8 *, u16 *, u32 *, u64 *, f32 *, f64 *, bool *>;

template <typename T>
concept fundamental = std::is_fundamental_v<T>;

/**
 * @brief 串口绘图器
 * @note  用于将一些变量的值格式化后用串口发给上位机绘图
 * @note  格式适配vofa的FireWater协议，如下
 * @note  "val1,val2,val3,...\\r\\n"
 *
 * @note  用法：
 * @note  1. 实例化一个SerialPlotter对象
 * @note  2. 调用AddVariable()添加需要绘制的变量
 * @note  3. 调用Update()更新绘图器数据
 * @note  4. 调用buffer()获取缓冲区指针
 * @note  5. 用任意手段把数据发给上位机，或者你想啥都不干也行
 * @note  6. 重复3-5
 */
class SerialPlotter {
 public:
  SerialPlotter() = default;
  void Update();
  [[nodiscard]] const std::string &buffer() const;
  template <typename T>
    requires fundamental<T>
  void AddVariable(T &variable);
  template <typename T>
    requires fundamental<T>
  void RemoveVariable(T &variable);

 private:
  std::ostringstream ostringstream_;
  std::string buffer_;
  std::list<VariableVariant> variable_list_;
};

/*********************/
/** Implementation ***/
/*********************/

/**
 * @brief   更新绘图器数据
 */
inline void SerialPlotter::Update() {
  if (this->variable_list_.empty()) {
    return;
  }

  this->buffer_.clear();
  this->ostringstream_.str("s:");

  for (const auto &var : this->variable_list_) {
    std::visit([this](auto &&arg) { this->ostringstream_ << std::fixed << std::setprecision(8) << *arg; }, var);
    if (&var == &this->variable_list_.back()) {
      this->ostringstream_ << "\r\n";
    } else {
      this->ostringstream_ << ",";
    }
  }
  this->buffer_ = this->ostringstream_.str();
}

/**
 * @brief   获取缓冲区
 * @return  缓冲区引用
 */
const inline std::string &SerialPlotter::buffer() const { return this->buffer_; }

/**
 * @brief   向绘图器注册一个变量
 * @tparam  T           变量类型
 * @param   variable    变量引用
 */
template <typename T>
  requires fundamental<T>
void SerialPlotter::AddVariable(T &variable) {
  void *this_var = nullptr;
  for (const auto &var : this->variable_list_) {
    std::visit([&this_var](auto &&arg) { this_var = reinterpret_cast<void *>(arg); }, var);
    if (reinterpret_cast<void *>(&variable) == this_var) {
      // 如果变量已经添加过，就不添加，直接返回
      return;
    }
  }

  this->variable_list_.emplace_back(&variable);
}

/**
 * @brief   从绘图器中移除一个变量
 * @tparam  T           变量类型
 * @param   variable    变量引用
 */
template <typename T>
  requires fundamental<T>
void SerialPlotter::RemoveVariable(T &variable) {
  void *this_var = nullptr;
  for (const auto &var : this->variable_list_) {
    std::visit([&this_var](auto &&arg) { this_var = reinterpret_cast<void *>(arg); }, var);
    if (reinterpret_cast<void *>(&variable)(&variable) == this_var) {
      this->variable_list_.remove(var);
      return;
    }
  }
}

}  // namespace irobot_ec::modules

#endif  // EC_LIB_MODULES_SERIAL_PLOTTER_HPP

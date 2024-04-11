
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
  template <typename T, typename SFINAE = std::enable_if_t<std::is_fundamental<T>::value>>
  void AddVariable(T &variable);
  template <typename T, typename SFINAE = std::enable_if_t<std::is_fundamental<T>::value>>
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
void irobot_ec::modules::SerialPlotter::Update() {
  if (this->variable_list_.empty()) {
    return;
  }

  this->buffer_.clear();
  this->ostringstream_.str("");

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
const std::string &SerialPlotter::buffer() const { return this->buffer_; }

/**
 * @brief   向绘图器注册一个变量
 * @tparam  T           变量类型
 * @param   variable    变量引用
 */
template <typename T, typename SFINAE>
void SerialPlotter::AddVariable(T &variable) {
  void *this_var = nullptr;
  for (const auto &var : this->variable_list_) {
    std::visit([&this_var](auto &&arg) { this_var = (void *)arg; }, var);
    if ((void *)(&variable) == this_var) {
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
template <typename T, typename SFINAE>
void SerialPlotter::RemoveVariable(T &variable) {
  void *this_var = nullptr;
  for (const auto &var : this->variable_list_) {
    std::visit([&this_var](auto &&arg) { this_var = (void *)arg; }, var);
    if ((void *)(&variable) == this_var) {
      this->variable_list_.remove(var);
      return;
    }
  }
}

}  // namespace irobot_ec::modules

#endif  // EC_LIB_MODULES_SERIAL_PLOTTER_HPP

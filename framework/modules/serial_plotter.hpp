
/**
 * @file  modules/serial_plotter.hpp
 * @brief 串口绘图器
 */

#ifndef EC_LIB_MODULES_SERIAL_PLOTTER_HPP
#define EC_LIB_MODULES_SERIAL_PLOTTER_HPP

#include <algorithm>
#include <iomanip>
#include <list>
#include <sstream>
#include <type_traits>
#include <typeinfo>

#include "modules/exception/exception.h"
#include "modules/typedefs.h"

namespace irobot_ec::modules {

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
  [[nodiscard]] const std::string *buffer() const;

 private:
  template <typename T, std::enable_if_t<std::is_fundamental<T>::value, int>>
  void AddVariable(T *variable);
  void RemoveVariable(void *variable);

  std::string buffer_;
  std::list<std::pair<std::type_info, void *>> variable_list_;
};

}  // namespace irobot_ec::modules

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

  for (auto it = this->variable_list_.begin(); it != this->variable_list_.end(); it++) {
    std::stringstream ss;

    if (it->first == typeid(int8_t)) {
      this->buffer_ += std::to_string(*((int8_t *)it->second));
    } else if (it->first == typeid(uint8_t)) {
      this->buffer_ += std::to_string(*((uint8_t *)it->second));
    } else if (it->first == typeid(int16_t)) {
      this->buffer_ += std::to_string(*((int16_t *)it->second));
    } else if (it->first == typeid(uint16_t)) {
      this->buffer_ += std::to_string(*((uint16_t *)it->second));
    } else if (it->first == typeid(int32_t)) {
      this->buffer_ += std::to_string(*((int32_t *)it->second));
    } else if (it->first == typeid(uint32_t)) {
      this->buffer_ += std::to_string(*((uint32_t *)it->second));
    } else if (it->first == typeid(int64_t)) {
      this->buffer_ += std::to_string(*((int64_t *)it->second));
    } else if (it->first == typeid(uint64_t)) {
      this->buffer_ += std::to_string(*((uint64_t *)it->second));
    } else if (it->first == typeid(fp32)) {
      ss << std::fixed << std::setprecision(7) << *((fp32 *)it->second);
      this->buffer_ += ss.str();
    } else if (it->first == typeid(fp64)) {
      ss << std::fixed << std::setprecision(7) << *((fp64 *)it->second);
      this->buffer_ += ss.str();
    }  // AddVariable函数保证变量一定是基本类型，所以不需要else

    if (it != --this->variable_list_.end()) {
      this->buffer_ += ",";
    } else {
      this->buffer_ += "\r\n";
    }
  }
}

/**
 * @brief   获取缓冲区指针
 * @return  缓冲区指针
 */
const std::string *irobot_ec::modules::SerialPlotter::buffer() const { return &this->buffer_; }

/**
 * @brief   向绘图器注册一个变量
 * @tparam  T           变量类型
 * @param   variable    变量指针
 */
template <typename T, std::enable_if_t<std::is_fundamental<T>::value, int>>
void irobot_ec::modules::SerialPlotter::AddVariable(T *variable) {
  // 如果变量已经添加过，就不添加，直接返回
  if (std::find_if(this->variable_list_.begin(), this->variable_list_.end(),
                   [variable](const std::pair<std::type_info, void *> &pair) { return pair.second == variable; }) !=
      this->variable_list_.end()) {
    return;
  }

  this->variable_list_.emplace_back(typeid(T), variable);
}

/**
 * @brief   从绘图器中移除一个变量
 * @param   variable    变量指针
 */
void irobot_ec::modules::SerialPlotter::RemoveVariable(void *variable) {
  this->variable_list_.remove_if(
      [variable](const std::pair<std::type_info, void *> &pair) { return pair.second == variable; });
}

#endif  // EC_LIB_MODULES_SERIAL_PLOTTER_HPP

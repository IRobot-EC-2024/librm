
/***
 * @file  modules/serial_plotter.hpp
 * @brief 串口绘图器
 */

#pragma once

#ifndef EC_LIB_MODULES_SERIAL_PLOTTER_HPP
#define EC_LIB_MODULES_SERIAL_PLOTTER_HPP

#include <iomanip>
#include <sstream>
#include <list>
#include <algorithm>

#include "modules/typedefs.h"
#include "modules/exceptions/exceptions.h"


namespace modules::serial_plotter {

    enum class VariableType {
        kI8,
        kU8,
        kI16,
        kU16,
        kI32,
        kU32,
        kI64,
        kU64,
        kFP32,
        kFP64,
    };


    /***
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
        template <typename T> void AddVariable(T *variable);
        void RemoveVariable(void *variable);

        std::string                                     buffer_;
        std::list<std::pair<VariableType, void *>>      variable_list_;
    };

}   // namespace modules::serial_plotter



/**********************/
/*** Implementation ***/
/**********************/


/***
 * @brief   更新绘图器数据
 */
void modules::serial_plotter::SerialPlotter::Update() {
    if (this->variable_list_.empty()) {
        return;
    }

    this->buffer_.clear();

    for (auto it = this->variable_list_.begin(); it != this->variable_list_.end(); it++) {
        std::stringstream ss;

        switch (it->first) {
            case VariableType::kI8:
                this->buffer_ += std::to_string(*((int8_t *)it->second));
                break;
            case VariableType::kU8:
                this->buffer_ += std::to_string(*((uint8_t *)it->second));
                break;
            case VariableType::kI16:
                this->buffer_ += std::to_string(*((int16_t *)it->second));
                break;
            case VariableType::kU16:
                this->buffer_ += std::to_string(*((uint16_t *)it->second));
                break;
            case VariableType::kI32:
                this->buffer_ += std::to_string(*((int32_t *)it->second));
                break;
            case VariableType::kU32:
                this->buffer_ += std::to_string(*((uint32_t *)it->second));
                break;
            case VariableType::kI64:
                this->buffer_ += std::to_string(*((int64_t *)it->second));
                break;
            case VariableType::kU64:
                this->buffer_ += std::to_string(*((uint64_t *)it->second));
                break;
            case VariableType::kFP32:
                ss << std::fixed << std::setprecision(7) << *((fp32 *)it->second);
                this->buffer_ += ss.str();
                break;
            case VariableType::kFP64:
                ss << std::fixed << std::setprecision(7) << *((fp64 *)it->second);
                this->buffer_ += ss.str();
                break;
        }

        if (it != --this->variable_list_.end()) {
            this->buffer_ += ",";
        } else {
            this->buffer_ += "\r\n";
        }
    }
}


/***
 * @brief   获取缓冲区指针
 * @return  缓冲区指针
 */
const std::string *modules::serial_plotter::SerialPlotter::buffer() const {
    return &this->buffer_;
}



/***
 * @brief   向绘图器注册一个变量
 * @tparam  T           变量类型
 * @param   variable    变量指针
 */
template <typename T>
void modules::serial_plotter::SerialPlotter::AddVariable(T *variable) {
    // 如果变量已经添加过，就不添加，直接返回
    if (std::find_if(this->variable_list_.begin(), this->variable_list_.end(),
                     [variable](const std::pair<VariableType, void *> &pair) {
                         return pair.second == variable;
                     }) != this->variable_list_.end()) {
        return;
    }

    if (typeid(T) == typeid(int8_t)) {
        this->variable_list_.push_back(std::make_pair(VariableType::kI8, variable));
    } else if (typeid(T) == typeid(uint8_t)) {
        this->variable_list_.push_back(std::make_pair(VariableType::kU8, variable));
    } else if (typeid(T) == typeid(int16_t)) {
        this->variable_list_.push_back(std::make_pair(VariableType::kI16, variable));
    } else if (typeid(T) == typeid(uint16_t)) {
        this->variable_list_.push_back(std::make_pair(VariableType::kU16, variable));
    } else if (typeid(T) == typeid(int32_t)) {
        this->variable_list_.push_back(std::make_pair(VariableType::kI32, variable));
    } else if (typeid(T) == typeid(uint32_t)) {
        this->variable_list_.push_back(std::make_pair(VariableType::kU32, variable));
    } else if (typeid(T) == typeid(int64_t)) {
        this->variable_list_.push_back(std::make_pair(VariableType::kI64, variable));
    } else if (typeid(T) == typeid(uint64_t)) {
        this->variable_list_.push_back(std::make_pair(VariableType::kU64, variable));
    } else if (typeid(T) == typeid(fp32)) {
        this->variable_list_.push_back(std::make_pair(VariableType::kFP32, variable));
    } else if (typeid(T) == typeid(fp64)) {
        this->variable_list_.push_back(std::make_pair(VariableType::kFP64, variable));
    } else {
        modules::exceptions::ThrowException(modules::exceptions::Exception::kTypeError);
    }
}



/***
 * @brief   从绘图器中移除一个变量
 * @param   variable    变量指针
 */
void modules::serial_plotter::SerialPlotter::RemoveVariable(void *variable) {
    this->variable_list_.remove_if([variable](const std::pair<VariableType, void *> &pair) {
        return pair.second == variable;
    });
}

#endif // EC_LIB_MODULES_SERIAL_PLOTTER_HPP

/* EOF */

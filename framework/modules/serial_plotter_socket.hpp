
/***
 * @file  app/serial_plotter_socket.hpp
 * @brief Serial plotter socket implementation
 */

#pragma once

#ifndef BALANCE_CHASSIS_SERIAL_PLOTTER_SOCKET_HPP
#define BALANCE_CHASSIS_SERIAL_PLOTTER_SOCKET_HPP

#include "usart.h"

#include "cmsis_os.h"

#include <iomanip>
#include <sstream>
#include <list>
#include <algorithm>

#include "typedefs.h"


extern "C" [[noreturn]] void debugPlotterTask(void const *pvParameter);


namespace modules::SerialPlotterSocket {

    enum variable_type_e {
        VARIABLE_TYPE_I8 = 0,
        VARIABLE_TYPE_U8,
        VARIABLE_TYPE_I16,
        VARIABLE_TYPE_U16,
        VARIABLE_TYPE_I32,
        VARIABLE_TYPE_U32,
        VARIABLE_TYPE_I64,
        VARIABLE_TYPE_U64,
        VARIABLE_TYPE_FP32,
        VARIABLE_TYPE_FP64
    };


    class SerialPlotterSocket {

    public:
        SerialPlotterSocket() = delete;
        explicit SerialPlotterSocket(UART_HandleTypeDef *huart);
        void update();

    private:
        template <typename T> void addVariable(T *variable);
        void removeVariable(void *variable);

        std::string                                     buffer_;
        UART_HandleTypeDef                             *huart_;
        std::list<std::pair<variable_type_e, void *>>   variable_list_;
    };

}   // namespace Modules::SerialPlotterSocket



/**********************/
/*** Implementation ***/
/**********************/


[[noreturn]] void debugPlotterTask(void const *pvParameter) {
    using namespace modules::SerialPlotterSocket;
    osDelay(1);

    auto *serialPlotterSocket = new SerialPlotterSocket(&huart1);

    while (true) {
        serialPlotterSocket->update();
        osDelay(100);
    }
}



modules::SerialPlotterSocket::SerialPlotterSocket::SerialPlotterSocket(UART_HandleTypeDef *huart) :
        huart_(huart) {}



void modules::SerialPlotterSocket::SerialPlotterSocket::update() {
    if (this->variable_list_.empty()) {
        return;
    }

    this->buffer_.clear();

    for (auto it = this->variable_list_.begin(); it != this->variable_list_.end(); it++) {
        std::stringstream ss;

        switch (it->first) {
            case VARIABLE_TYPE_I8:
                this->buffer_ += std::to_string(*((int8_t *)it->second));
                break;
            case VARIABLE_TYPE_U8:
                this->buffer_ += std::to_string(*((uint8_t *)it->second));
                break;
            case VARIABLE_TYPE_I16:
                this->buffer_ += std::to_string(*((int16_t *)it->second));
                break;
            case VARIABLE_TYPE_U16:
                this->buffer_ += std::to_string(*((uint16_t *)it->second));
                break;
            case VARIABLE_TYPE_I32:
                this->buffer_ += std::to_string(*((int32_t *)it->second));
                break;
            case VARIABLE_TYPE_U32:
                this->buffer_ += std::to_string(*((uint32_t *)it->second));
                break;
            case VARIABLE_TYPE_I64:
                this->buffer_ += std::to_string(*((int64_t *)it->second));
                break;
            case VARIABLE_TYPE_U64:
                this->buffer_ += std::to_string(*((uint64_t *)it->second));
                break;
            case VARIABLE_TYPE_FP32:
                ss << std::fixed << std::setprecision(7) << *((float *)it->second);
                this->buffer_ += ss.str();
                break;
            case VARIABLE_TYPE_FP64:
                ss << std::fixed << std::setprecision(15) << *((double *)it->second);
                this->buffer_ += ss.str();
                break;
        }

        if (it != --this->variable_list_.end()) {
            this->buffer_ += ",";
        } else {
            this->buffer_ += "\r\n";
        }
    }

    while (HAL_UART_GetState(this->huart_) != HAL_UART_STATE_READY) {
        // wait for DMA to be ready
    }

    HAL_UART_Transmit_DMA(this->huart_, (uint8_t *)this->buffer_.c_str(), this->buffer_.length());
}



template <typename T>
void modules::SerialPlotterSocket::SerialPlotterSocket::addVariable(T *variable) {
    if (std::find_if(this->variable_list_.begin(), this->variable_list_.end(),
                     [variable](const std::pair<variable_type_e, void *> &pair) {
                         return pair.second == variable;
                     }) != this->variable_list_.end()) {
        return;
    }

    if (typeid(T) == typeid(int8_t)) {
        this->variable_list_.push_back(std::make_pair(VARIABLE_TYPE_I8, variable));
    } else if (typeid(T) == typeid(uint8_t)) {
        this->variable_list_.push_back(std::make_pair(VARIABLE_TYPE_U8, variable));
    } else if (typeid(T) == typeid(int16_t)) {
        this->variable_list_.push_back(std::make_pair(VARIABLE_TYPE_I16, variable));
    } else if (typeid(T) == typeid(uint16_t)) {
        this->variable_list_.push_back(std::make_pair(VARIABLE_TYPE_U16, variable));
    } else if (typeid(T) == typeid(int32_t)) {
        this->variable_list_.push_back(std::make_pair(VARIABLE_TYPE_I32, variable));
    } else if (typeid(T) == typeid(uint32_t)) {
        this->variable_list_.push_back(std::make_pair(VARIABLE_TYPE_U32, variable));
    } else if (typeid(T) == typeid(int64_t)) {
        this->variable_list_.push_back(std::make_pair(VARIABLE_TYPE_I64, variable));
    } else if (typeid(T) == typeid(uint64_t)) {
        this->variable_list_.push_back(std::make_pair(VARIABLE_TYPE_U64, variable));
    } else if (typeid(T) == typeid(fp32)) {
        this->variable_list_.push_back(std::make_pair(VARIABLE_TYPE_FP32, variable));
    } else if (typeid(T) == typeid(fp64)) {
        this->variable_list_.push_back(std::make_pair(VARIABLE_TYPE_FP64, variable));
    }
}



void modules::SerialPlotterSocket::SerialPlotterSocket::removeVariable(void *variable) {
    this->variable_list_.remove_if([variable](const std::pair<variable_type_e, void *> &pair) {
        return pair.second == variable;
    });
}

#endif

/* EOF */

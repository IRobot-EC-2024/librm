/**
 * @file    components/sensor/ist8310_const.hpp
 * @brief   IST8310有关常量
 */

#ifndef EC_LIB_COMPONENTS_SENSOR_IST8310_IST8310_CONST_H_
#define EC_LIB_COMPONENTS_SENSOR_IST8310_IST8310_CONST_H_

#include "modules/typedefs.h"

constexpr u8 IST8310_I2C_ADDRESS = 0x0E;
constexpr u8 IST8310_WHO_AM_I = 0x00;        // ist8310 "who am I"
constexpr u8 IST8310_WHO_AM_I_VALUE = 0x10;  // device ID

constexpr u8 IST8310_COMM_WAIT_TIME_US = 150;
constexpr u8 IST8310_COMM_WAIT_TIME_MS = 50;

constexpr u8 IST8310_DATA_READY_BIT = 2;

constexpr u8 IST8310_DATA_OUT_X_L = 0x03;

constexpr f32 IST8310_SENSITIVITY = 0.3f;  // 原始数据到单位ut的换算系数

#endif  // EC_LIB_COMPONENTS_SENSOR_IST8310_IST8310_CONST_H_

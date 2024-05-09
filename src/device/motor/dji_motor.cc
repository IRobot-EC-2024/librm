/**
 * @file  device/motor/dji_motor.cc
 * @brief 大疆电机类库
 */

#include "hal/hal.h"
#if defined(HAL_CAN_MODULE_ENABLED)

#include "dji_motor.hpp"

namespace irobot_ec::device {

/**
 * @brief 三种型号电机各自的发送缓冲区
 */
std::unordered_map<hal::CanBase *, std::array<u8, 18>> DjiMotorProperties<DjiMotorType::GM6020>::tx_buf_{};
std::unordered_map<hal::CanBase *, std::array<u8, 18>> DjiMotorProperties<DjiMotorType::M3508>::tx_buf_{};
std::unordered_map<hal::CanBase *, std::array<u8, 18>> DjiMotorProperties<DjiMotorType::M2006>::tx_buf_{};

}  // namespace irobot_ec::device

#endif
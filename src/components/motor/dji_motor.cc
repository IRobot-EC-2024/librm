/**
 * @file  components/motor/dji/dji_motor.cc
 * @brief 大疆电机的类封装
 */

#include "hal/hal.h"
#if defined(HAL_CAN_MODULE_ENABLED)

#include "dji_motor.hpp"

namespace irobot_ec::components {

/**
 * @brief 三种型号电机各自的发送缓冲区
 */
std::unordered_map<bsp::CanInterface *, std::array<u8, 18>> DjiMotorProperties<DjiMotorType::GM6020>::tx_buf_{};
std::unordered_map<bsp::CanInterface *, std::array<u8, 18>> DjiMotorProperties<DjiMotorType::M3508>::tx_buf_{};
std::unordered_map<bsp::CanInterface *, std::array<u8, 18>> DjiMotorProperties<DjiMotorType::M2006>::tx_buf_{};

/**
 * @brief 所有电机对象
 */
std::list<DjiMotorBase *> DjiMotorBase::motor_list_{};

/**
 * @param can CAN总线对象
 * @param id  电机ID(1~8)[GM6020是1~7]
 */
DjiMotorBase::DjiMotorBase(bsp::CanInterface &can, u16 id) : bsp::CanDeviceBase(can, id) {}

}  // namespace irobot_ec::components

#endif
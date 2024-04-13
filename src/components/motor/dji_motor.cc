/**
 * @file  components/motor/dji_motor.cc
 * @brief 大疆电机的类封装
 */

#include "hal/hal.h"
#if defined(HAL_CAN_MODULE_ENABLED)

#include "dji_motor.hpp"

namespace irobot_ec::components {
/**
 * @brief 三种型号电机各自的发送缓冲区
 */
std::unordered_map<bsp::CanBase *, std::array<u8, 18>> DjiMotorProperties<DjiMotorType::GM6020>::tx_buf_{};
std::unordered_map<bsp::CanBase *, std::array<u8, 18>> DjiMotorProperties<DjiMotorType::M3508>::tx_buf_{};
std::unordered_map<bsp::CanBase *, std::array<u8, 18>> DjiMotorProperties<DjiMotorType::M2006>::tx_buf_{};

/**
 * @brief 存放所有电机对象的链表
 */
std::list<DjiMotorBase *> DjiMotorBase::motor_list_;

/**
 * @param can CAN总线对象
 * @param id  电机ID(1~8)[GM6020是1~7]
 */
DjiMotorBase::DjiMotorBase(bsp::CanBase &can, u16 id) : bsp::CanDeviceBase(can, id) {}

/**
 * @brief  向所有电机发出控制消息
 */
void DjiMotorBase::SendCommand() {
  for (const auto &motor : DjiMotorBase::motor_list_) {
    // 遍历所有电机对象，检查标志位，对应缓冲区的标志位为1则发送，然后将标志位清零
    if (motor->tx_buf_->at(16) == 1) {
      // 发前八字节
      motor->can_->Write(motor->control_id_[0], motor->tx_buf_->data(), 8);
      motor->tx_buf_->at(16) = 0;
    }
    if (motor->tx_buf_->at(17) == 1) {
      // 发后八字节
      motor->can_->Write(motor->control_id_[1], motor->tx_buf_->data() + 8, 8);
      motor->tx_buf_->at(17) = 0;
    }
  }
}

}  // namespace irobot_ec::components

#endif
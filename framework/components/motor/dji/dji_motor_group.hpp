
/**
 * @file  components/motor/dji/dji_motor_group.hpp
 * @brief 大疆电机组
 * @note  得益于大疆电机的通信协议，一条CAN报文可以控制最多4个电机。
 *        本文件中的"电机组"类实现了这种特性，可以避免控制几个电机就要发送几条报文的情况。
 *        若有控制一条总线上多个同型号电机的需求，建议使用本类把多个电机对象编组。
 */

#ifndef EC_LIB_COMPONENTS_MOTOR_DJI_MOTOR_GROUP_HPP
#define EC_LIB_COMPONENTS_MOTOR_DJI_MOTOR_GROUP_HPP

#include <initializer_list>
#include <type_traits>
#include <unordered_map>

#include "dji_motor.h"
#include "modules/exception/exception.h"
#include "modules/typedefs.h"

namespace irobot_ec::components::motor {

/**
 * @brief  大疆电机组
 * @tparam MotorType 电机类型，必须是DjiMotorBase的子类(GM6020, M3508或M2006)
 */
template <typename MotorType>
class DjiMotorGroup {
  static_assert(std::is_base_of<DjiMotorBase, MotorType>::value, "MotorType must be subclass of DjiMotorBase");

 public:
  DjiMotorGroup() = delete;
  DjiMotorGroup(std::initializer_list<MotorType *> motors);

  // no copy
  DjiMotorGroup(const DjiMotorGroup &) = delete;
  DjiMotorGroup &operator=(const DjiMotorGroup &) = delete;

  void SetCurrents(const std::unordered_map<uint8_t, int16_t> &currents);
  void SetCurrents(std::unordered_map<uint8_t, int16_t> &&currents);

 private:
  template <typename T>
  void SetCurrentsTemplate(T &&currents);

  uint8_t tx_buf_index_{};
  std::unordered_map<uint8_t, MotorType *> motors_{};  // 电机ID -> 电机对象指针
};

/****************************/
// Implementations
/****************************/

/**
 * @tparam MotorType 电机类型
 * @param  motors    电机对象列表
 */
template <typename MotorType>
DjiMotorGroup<MotorType>::DjiMotorGroup(std::initializer_list<MotorType *> motors) {
  for (const auto &motor : motors) {
    this->motors_[motor->id_] = motor;
  }

  if (std::is_same<MotorType, GM6020>::value) {
    this->tx_buf_index_ = 0;
  } else if (std::is_same<MotorType, M3508>::value) {
    this->tx_buf_index_ = 1;
  } else if (std::is_same<MotorType, M2006>::value) {
    this->tx_buf_index_ = 2;
  }
}

/**
 * @tparam MotorType 电机类型
 * @tparam T         std::unordered_map<uint8_t, int16_t>，写成模板是为了同时支持左值引用和右值引用
 * @param  currents  map(电机ID, 电流值)
 */
template <typename MotorType>
template <typename T>
void DjiMotorGroup<MotorType>::SetCurrentsTemplate(T &&currents) {
  uint8_t target_motors = 0b00000000;  // 标记要控制的电机，高位->低位对应电机ID从1到8

  for (const auto &current : currents) {
    if (this->motors_.find(current.first) != this->motors_.end()) {
      target_motors |= 1 << (current.first - 1);
      this->motors_[current.first]->SetCurrent(current.second, false);
    } else {
      irobot_ec::modules::exception::ThrowException(irobot_ec::modules::exception::Exception::kKeyError);
    }
  }

  if (target_motors | 0b11110000) {
    // 存在id为1~4的电机，发缓冲区前八个字节
  }

  if (target_motors | 0b00001111) {
    // 存在id为5~8的电机，发缓冲区后八个字节
  }
}

/**
 * @brief  设置电机组的电流
 * @tparam MotorType    电机类型
 * @param  currents     map(电机ID, 电流值)
 */
template <typename MotorType>
void DjiMotorGroup<MotorType>::SetCurrents(const std::unordered_map<uint8_t, int16_t> &currents) {
  this->SetCurrentsTemplate(currents);
}

/**
 * @brief  设置电机组的电流
 * @tparam MotorType    电机类型
 * @param  currents     map(电机ID, 电流值)
 */
template <typename MotorType>
void DjiMotorGroup<MotorType>::SetCurrents(std::unordered_map<uint8_t, int16_t> &&currents) {
  this->SetCurrentsTemplate(std::move(currents));
}

}  // namespace irobot_ec::components::motor

#endif  // EC_LIB_COMPONENTS_MOTOR_DJI_MOTOR_GROUP_HPP

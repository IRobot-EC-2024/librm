
// TODO: 未完成

/**
 * @file  components/motor/dji/dji_motor_group.hpp
 * @brief 大疆电机组
 * @note  得益于大疆电机的通信协议，一条CAN报文可以控制最多4个电机。
 *        本文件中的"电机组"类封装了这种特性，可以减少CAN总线上的消息数量。
 *        若有控制多个电机的需求，建议使用本文件中的类，而不是实例化多个电机对象分别控制。
 */

#ifndef EC_LIB_COMPONENTS_MOTOR_DJI_MOTOR_GROUP_HPP
#define EC_LIB_COMPONENTS_MOTOR_DJI_MOTOR_GROUP_HPP

#include <vector>

#include "dji_motor.h"
#include "modules/typedefs.h"

namespace irobot_ec::components::motor {

class DjiMotorGroup final {
 public:
 private:
  ::std::vector<DjiMotorBase> motors_;
};

}  // namespace irobot_ec::components::motor

#endif  // EC_LIB_COMPONENTS_MOTOR_DJI_MOTOR_GROUP_HPP

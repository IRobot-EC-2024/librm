/**
 * @file  components/motor/unitree_motor.h
 * @brief 宇树电机类库
 * @todo  未完成
 */

#ifndef EC_LIB_COMPONENTS_MOTOR_UNITREE_MOTOR_H
#define EC_LIB_COMPONENTS_MOTOR_UNITREE_MOTOR_H

#include "hal/hal.h"
#if defined(HAL_UART_MODULE_ENABLED)

#include "modules/typedefs.h"

namespace irobot_ec::components::motor {

enum class UnitreeMotorMode {
  kLock = 0x00,
  kFOC = 0x01,
  kCalibration = 0x02,
  kReserved1 = 0x03,
  kReserved2 = 0x04,
  kReserved3 = 0x05,
  kReserved4 = 0x06,
  kReserved5 = 0x07,
};

class UnitreeMotorBase {
 public:
  UnitreeMotorBase() = delete;
  ~UnitreeMotorBase() = default;

 protected:
  u8 tx_buffer_[17];
  u16 id_{};

  /** MOTOR FEEDBACK DATA **/
  UnitreeMotorMode mode_{};
  i16 tau_{};       // Divide by 256 to get the real value(N*m)
  i16 velocity_{};  // Divide by 1000 to get the real value(rad/s)

  /*************************/
};

}  // namespace irobot_ec::components::motor

#endif

#endif  // EC_LIB_COMPONENTS_MOTOR_UNITREE_MOTOR_H

/* EOF */

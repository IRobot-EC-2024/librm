
/***
 * @file  components/c_motor.h
 * @brief Motor driver classes
 */

#ifndef EC_LIB_COMPONENTS_MOTOR_DJI_MOTOR_H
#define EC_LIB_COMPONENTS_MOTOR_DJI_MOTOR_H

#include "hal_wrapper/hal_config.h"

#ifdef HAL_CAN_MODULE_ENABLED

#include "hal_wrapper/hal_can.h"
#include "modules/typedefs.h"

namespace components::motor {

/***
 * @brief DJI motor base class
 * @note This class is pure virtual, its inheritance class must implement
 * @note SetCurrent() to control the motor
 */
class DjiMotorBase : public hal::can::CanDeviceBase {
public:
  DjiMotorBase() = delete;
  DjiMotorBase(CAN_HandleTypeDef *hcan, uint16_t id);

  virtual void SetCurrent(int16_t current) = 0;
  virtual void PushControlMessage() = 0;

  [[nodiscard]] uint16_t encoder() const;
  [[nodiscard]] uint16_t rpm() const;
  [[nodiscard]] uint16_t current() const;
  [[nodiscard]] uint16_t temperature() const;

protected:
  void RxCallback(hal::can::can_rx_msg_t *msg) override;

  uint16_t id_{};
  /** MOTOR FEEDBACK DATA **/
  uint16_t encoder_{};
  uint16_t rpm_{};
  uint16_t current_{};
  uint16_t temperature_{};
  /*************************/
};

/***
 * @brief DJI GM6020 motor
 * @note Feedback message stdID:    0x205 + ID
 * @note Control message stdID:     0x1ff(1234), 0x2ff(567)
 * @note Voltage range:             -30000 ~ 30000, big-endian
 * @note STD, DATA, DLC=8
 * @note https://www.robomaster.com/zh-CN/products/components/general/GM6020
 */
class GM6020 final : public DjiMotorBase {
public:
  GM6020() = delete;
  GM6020(CAN_HandleTypeDef *hcan, uint16_t id);

  void SetCurrent(int16_t current) override; // GM6020的current实际上是电压
  void PushControlMessage() override;

protected:
  static uint8_t tx_buffer_[16];
};

/***
 * @brief DJI M2006 Motor/C610 ESC
 * @note Feedback message stdID:    0x200 + ID
 * @note Control message stdID:     0x200(1234), 0x1ff(5678)
 * @note Current range:             -10000 ~ 10000(-10A ~ 10A), big-endian
 * @note STD, DATA, DLC=8
 * @note https://www.robomaster.com/zh-CN/products/components/general/M2006
 */
class M2006 final : public DjiMotorBase {
public:
  M2006() = delete;
  M2006(CAN_HandleTypeDef *hcan, uint16_t id);

  void SetCurrent(int16_t current) override;
  void PushControlMessage() override;

protected:
  static uint8_t tx_buffer_[16];
};

/***
 * @brief DJI M3508 Motor/C620 ESC
 * @note Feedback message stdID:    0x201 + ID
 * @note Control message stdID:     0x200(1234), 0x1ff(5678)
 * @note Current range:             -16384 ~ 16384(-20A ~ 20A), big-endian
 * @note STD, DATA, DLC=8
 * @note https://www.robomaster.com/zh-CN/products/components/general/M3508
 */
class M3508 final : public DjiMotorBase {
public:
  M3508() = delete;
  M3508(CAN_HandleTypeDef *hcan, uint16_t id);

  void SetCurrent(int16_t current) override;
  void PushControlMessage() override;

protected:
  static uint8_t tx_buffer_[16];
};

} // namespace components::motor

#endif

#endif // EC_LIB_COMPONENTS_MOTOR_DJI_MOTOR_H

/* EOF */

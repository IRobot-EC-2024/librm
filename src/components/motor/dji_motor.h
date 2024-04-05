
/**
 * @file  components/motor/dji/dji_motor.h
 * @brief 大疆电机的类封装
 */

#ifndef EC_LIB_COMPONENTS_MOTOR_DJI_MOTOR_H
#define EC_LIB_COMPONENTS_MOTOR_DJI_MOTOR_H

#include "hal_wrapper/hal.h"

#ifdef HAL_CAN_MODULE_ENABLED

#include <array>
#include <list>
#include <unordered_map>

#include "hal_wrapper/can_device.h"
#include "modules/typedefs.h"

namespace irobot_ec::components::motor {

/**
 * @brief     DJI电机的基类
 * @attention 本类是抽象类，不可实例化
 * @attention 子类必须实现SetCurrent函数，用于设置电机的电流/电压
 */

class DjiMotorBase : public irobot_ec::hal::can::CanDeviceBase {
 public:
  DjiMotorBase() = delete;
  static void SendCommand();

  /* 取值函数 */
  [[nodiscard]] uint16_t encoder() const;
  [[nodiscard]] int16_t rpm() const;
  [[nodiscard]] int16_t current() const;
  [[nodiscard]] uint8_t temperature() const;
  /***********/

 protected:
  DjiMotorBase(uint8_t buffer_index, CAN_HandleTypeDef *hcan, uint16_t id);

  template <int16_t current_bound, uint8_t buffer_index>
  void SetCurrentTemplate(int16_t current);
  void RxCallback(bsp::CanRxMsg *msg) override;

  uint16_t id_{};  // 电机ID
  /**   FEEDBACK DATA   **/
  uint16_t encoder_{};     // 电机编码器值
  int16_t rpm_{};          // 电机转速
  int16_t current_{};      // 电机实际电流
  uint8_t temperature_{};  // 电机温度
  /***********************/

  /**
   * @note 三个型号的电机各自的发送缓冲区
   * @note 每种电机对应一个缓冲区组，每个缓冲区组是一个键值对字典(can总线指针, 18字节的缓冲区数组)
   * @note 缓冲区的前16字节是电机控制数据，最后两字节是发送标志(buf[17]: 前八字节是否发送, buf[16]: 后八字节是否发送)
   */
  static std::array<std::unordered_map<CAN_HandleTypeDef *, std::array<uint8_t, 18>> *, 3> tx_bufs_;
  std::array<uint8_t, 18> *tx_buffer_p_;  // 指向这个电机发送缓冲区的指针

 private:
  static std::list<DjiMotorBase *> motor_list_;  // 电机对象链表
};

/**
 * @brief DJI GM6020电机
 * @note 反馈报文stdID:     0x205 + ID
 * @note 控制报文stdID:     0x1ff(1234), 0x2ff(567)
 * @note Voltage range:   -30000 ~ 30000, big-endian
 * @note STD, DATA, DLC=8
 * @note https://www.robomaster.com/zh-CN/products/components/general/GM6020
 */
class GM6020 final : public DjiMotorBase {
 public:
  GM6020() = delete;
  GM6020(CAN_HandleTypeDef *hcan, uint16_t id);

  // no copy constructor
  GM6020(const GM6020 &) = delete;
  GM6020 &operator=(const GM6020 &) = delete;

  void SetCurrent(int16_t current);
};

/**
 * @brief DJI M2006电机/C610电调
 * @note 反馈报文stdID:     0x200 + ID
 * @note 控制报文stdID:     0x200(1234), 0x1ff(5678)
 * @note 电流范围:          -10000 ~ 10000(-10A ~ 10A), big-endian
 * @note STD, DATA, DLC=8
 * @note https://www.robomaster.com/zh-CN/products/components/general/M2006
 */
class M2006 final : public DjiMotorBase {
 public:
  M2006() = delete;
  M2006(CAN_HandleTypeDef *hcan, uint16_t id);

  // no copy constructor
  M2006(const M2006 &) = delete;
  M2006 &operator=(const M2006 &) = delete;

  void SetCurrent(int16_t current);
};

/**
 * @brief DJI M3508电机/C620电调
 * @note 反馈报文stdID:     0x201 + ID
 * @note 控制报文stdID:     0x200(1234), 0x1ff(5678)
 * @note 电流范围:         -16384 ~ 16384(-20A ~ 20A), big-endian
 * @note STD, DATA, DLC=8
 * @note https://www.robomaster.com/zh-CN/products/components/general/M3508
 */
class M3508 final : public DjiMotorBase {
 public:
  M3508() = delete;
  M3508(CAN_HandleTypeDef *hcan, uint16_t id);

  // no copy constructor
  M3508(const M3508 &) = delete;
  M3508 &operator=(const M3508 &) = delete;

  void SetCurrent(int16_t current);
};

}  // namespace irobot_ec::components::motor

#endif

#endif  // EC_LIB_COMPONENTS_MOTOR_DJI_MOTOR_H

/* EOF */

/**
 * @file  device/motor/dji_motor.hpp
 * @brief 大疆电机类库
 *
 * @note DJI GM6020电机
 * @note 反馈报文stdID:     0x205 + ID
 * @note 控制报文stdID:     0x1ff(1234), 0x2ff(567)
 * @note Voltage range:   -30000 ~ 30000, big-endian
 * @note STD, DATA, DLC=8
 * @note https://www.robomaster.com/zh-CN/products/components/general/GM6020
 *
 * @note DJI M3508电机/C620电调
 * @note 反馈报文stdID:     0x200 + ID
 * @note 控制报文stdID:     0x200(1234), 0x1ff(5678)
 * @note 电流范围:         -16384 ~ 16384(-20A ~ 20A), big-endian
 * @note STD, DATA, DLC=8
 * @note https://www.robomaster.com/zh-CN/products/components/general/M3508
 *
 * @note DJI M2006电机/C610电调
 * @note 反馈报文stdID:     0x200 + ID
 * @note 控制报文stdID:     0x200(1234), 0x1ff(5678)
 * @note 电流范围:          -10000 ~ 10000(-10A ~ 10A), big-endian
 * @note STD, DATA, DLC=8
 * @note https://www.robomaster.com/zh-CN/products/components/general/M2006
 */

#ifndef EC_LIB_COMPONENTS_MOTOR_DJI_MOTOR_HPP
#define EC_LIB_COMPONENTS_MOTOR_DJI_MOTOR_HPP

#include "hal/hal.h"
#if defined(HAL_CAN_MODULE_ENABLED)

#include <array>
#include <unordered_map>

#include "modules/typedefs.h"
#include "modules/algorithm/utils.hpp"
#include "hal/can.h"

namespace irobot_ec::device {

enum class DjiMotorType { Default, GM6020, M3508, M2006 };
template <DjiMotorType motor_type>
struct DjiMotorProperties {};

template <>
struct DjiMotorProperties<DjiMotorType::GM6020> {
  static constexpr u16 kRxIdBase = 0x205;
  static constexpr u16 kControlId[2] = {0x1ff, 0x2ff};
  static constexpr i16 kCurrentBound = 30000;
  static std::unordered_map<hal::CanBase *, std::array<u8, 18>> tx_buf_;
};

template <>
struct DjiMotorProperties<DjiMotorType::M3508> {
  static constexpr u16 kRxIdBase = 0x200;
  static constexpr u16 kControlId[2] = {0x200, 0x1ff};
  static constexpr i16 kCurrentBound = 16384;
  static std::unordered_map<hal::CanBase *, std::array<u8, 18>> tx_buf_;
};

template <>
struct DjiMotorProperties<DjiMotorType::M2006> {
  static constexpr u16 kRxIdBase = 0x200;
  static constexpr u16 kControlId[2] = {0x200, 0x1ff};
  static constexpr i16 kCurrentBound = 10000;
  static std::unordered_map<hal::CanBase *, std::array<u8, 18>> tx_buf_;
};

/**
 * @brief     DJI电机
 * @attention 本类是抽象类，不可实例化
 * @attention 子类必须实现SetCurrent函数，用于设置电机的电流/电压
 */
template <DjiMotorType motor_type = DjiMotorType::Default>
class DjiMotor final : public CanDeviceBase {
 public:
  DjiMotor() = delete;
  ~DjiMotor() override = default;
  DjiMotor(hal::CanBase &can, u16 id);

  // 禁止拷贝构造
  DjiMotor(const DjiMotor &) = delete;
  DjiMotor &operator=(const DjiMotor &) = delete;

  void SetCurrent(i16 current);
  static void SendCommand();

  /** 取值函数 **/
  [[nodiscard]] u16 encoder() const { return this->encoder_; }
  [[nodiscard]] i16 rpm() const { return this->rpm_; }
  [[nodiscard]] i16 current() const { return this->current_; }
  [[nodiscard]] u8 temperature() const { return this->temperature_; }
  /*************/

 private:
  void RxCallback(const hal::CanRxMsg *msg) override;

  u16 id_{};  // 电机ID
  /**   FEEDBACK DATA   **/
  u16 encoder_{};     // 电机编码器值
  i16 rpm_{};         // 电机转速
  i16 current_{};     // 电机实际电流
  u8 temperature_{};  // 电机温度
  /***********************/
};

using GM6020 = DjiMotor<DjiMotorType::GM6020>;
using M3508 = DjiMotor<DjiMotorType::M3508>;
using M2006 = DjiMotor<DjiMotorType::M2006>;

/**
 * @tparam motor_type 电机类型(GM6020, M3508, M2006)
 * @param  can        指向CAN总线对象的指针
 * @param  id
 */
template <DjiMotorType motor_type>
DjiMotor<motor_type>::DjiMotor(hal::CanBase &can, u16 id)
    : CanDeviceBase(can, DjiMotorProperties<motor_type>::kRxIdBase + id), id_(id) {
  // 如果这个电机对象所在CAN总线的发送缓冲区还未创建，就创建一个
  if (DjiMotorProperties<motor_type>::tx_buf_.find(&can) == DjiMotorProperties<motor_type>::tx_buf_.end()) {
    DjiMotorProperties<motor_type>::tx_buf_.insert({&can, {0}});
  }
}

/**
 * @brief  设置电机的输出电流，本模板函数留给子类实例化，设定电流范围
 * @note   这个函数不会发送控制消息，在设置电流值后需要调用SendCommand函数向所有电机发出控制消息
 * @tparam motor_type 电机类型
 * @param  current    设定电流值
 */
template <DjiMotorType motor_type>
void DjiMotor<motor_type>::SetCurrent(i16 current) {
  // 限幅到电机能接受的最大电流
  current = modules::algorithm::utils::absConstrain(current, DjiMotorProperties<motor_type>::kCurrentBound);
  // 根据这个电机所属的can总线(this->can_)和电机ID(this->id_)找到对应的发送缓冲区，写入电流值
  DjiMotorProperties<motor_type>::tx_buf_[this->can_].at((this->id_ - 1) * 2) = (current >> 8) & 0xff;
  DjiMotorProperties<motor_type>::tx_buf_[this->can_].at((this->id_ - 1) * 2 + 1) = current & 0xff;
  // 根据电机ID判断缓冲区前八位需要发送，还是后八位需要发送，把对应的标志位置1
  if (1 <= this->id_ && this->id_ <= 4) {
    DjiMotorProperties<motor_type>::tx_buf_[this->can_].at(16) = 1;
  } else if (5 <= this->id_ && this->id_ <= 8) {
    DjiMotorProperties<motor_type>::tx_buf_[this->can_].at(17) = 1;
  }
}

/**
 * @brief 向对应型号的所有电机发出控制消息
 */
template <DjiMotorType motor_type>
void DjiMotor<motor_type>::SendCommand() {
  for (auto &buf : DjiMotorProperties<motor_type>::tx_buf_) {
    if (buf.second.at(16) == 1) {
      buf.first->Write(DjiMotorProperties<motor_type>::kControlId[0], buf.second.data(), 8);
      buf.second.at(16) = 0;
    }
    if (buf.second.at(17) == 1) {
      buf.first->Write(DjiMotorProperties<motor_type>::kControlId[1], buf.second.data() + 8, 8);
      buf.second.at(17) = 0;
    }
  }
}

/**
 * @brief  向所有大疆电机发出控制消息
 */
template <>
inline void DjiMotor<DjiMotorType::Default>::SendCommand() {
  GM6020::SendCommand();
  M3508::SendCommand();
  M2006::SendCommand();
}

/**
 * @brief  CAN回调函数，解码收到的反馈报文
 * @tparam motor_type 电机类型
 * @param  msg        收到的消息
 */
template <DjiMotorType motor_type>
void DjiMotor<motor_type>::RxCallback(const hal::CanRxMsg *msg) {
  this->encoder_ = (msg->data[0] << 8) | msg->data[1];
  this->rpm_ = (msg->data[2] << 8) | msg->data[3];
  this->current_ = (msg->data[4] << 8) | msg->data[5];
  this->temperature_ = msg->data[6];
}

}  // namespace irobot_ec::device

#endif

#endif  // EC_LIB_COMPONENTS_MOTOR_DJI_MOTOR_HPP


/***
 * @file  components/motor/dji/dji_motor.cpp
 * @brief 大疆电机驱动的实现
 */

#include "hal_wrapper/hal_config.h"

#ifdef HAL_CAN_MODULE_ENABLED

#include "dji_motor.h"
#include "modules/algorithm/utils/utils.hpp"
#include "modules/exceptions/exceptions.h"

using namespace components::motor;
using modules::exceptions::Exception;
using modules::exceptions::ThrowException;

/****************************
 * CLASS DJIMotorBase
 ****************************/

/***
 * @brief 构造函数
 * @param hcan  指向CAN总线对象的指针
 * @param id    电机ID
 */
DjiMotorBase::DjiMotorBase(CAN_HandleTypeDef *hcan, uint16_t rx_std_id)
    : hal::can::CanDeviceBase(hcan, rx_std_id) {}

/***
 * @brief   电机反馈数据解码回调函数
 */
void DjiMotorBase::RxCallback(hal::can::CanRxMsg *msg) {
  this->encoder_ = (msg->data[0] << 8) | msg->data[1];
  this->rpm_ = (msg->data[2] << 8) | msg->data[3];
  this->current_ = (msg->data[4] << 8) | msg->data[5];
  this->temperature_ = (msg->data[6] << 8) | msg->data[7];
}

/***
 * @brief   获取电机的编码器值
 * @return  编码器值(0~8191 => 0~360°)
 */
uint16_t DjiMotorBase::encoder() const { return this->encoder_; }

/***
 * @brief   获取电机的转速
 * @return  RPM(rad/s)
 */
uint16_t DjiMotorBase::rpm() const { return this->rpm_; }

/***
 * @brief   获取电机的实际电流
 * @return  电流值(无单位)
 */
uint16_t DjiMotorBase::current() const { return this->current_; }

/***
 * @brief   获取电机的温度
 * @return  温度(°C)
 */
uint16_t DjiMotorBase::temperature() const { return this->temperature_; }

/****************************
 * CLASS GM6020
 ****************************/

uint8_t components::motor::GM6020::tx_buffer_[16] = {0};

/***
 * @brief 构造函数
 * @param hcan  指向CAN总线对象的指针
 * @param id    电机ID(1~7)
 */
GM6020::GM6020(CAN_HandleTypeDef *hcan, uint16_t id)
    : DjiMotorBase(hcan, 0x204 + id) {
  if (1 <= id && id <= 7) {
    this->id_ = id;
  } else {
    ThrowException(Exception::kValueError);  // 电机ID超出范围
  }
}

/***
 * @brief 设置电机的输出电流
 * @note  这个函数不会发送控制消息，需要调用PushControlMessage()函数推送
 * @param current   电流值(-30000 ~ 30000)
 */
void GM6020::SetCurrent(int16_t current) {
  current = modules::algorithm::utils::absConstrain(current, (int16_t)30000);

  if (1 <= this->id_ && this->id_ <= 7) {
    GM6020::tx_buffer_[(this->id_ - 1) * 2] = (current >> 8) & 0xff;
    GM6020::tx_buffer_[(this->id_ - 1) * 2 + 1] = current & 0xff;
  } else {
    ThrowException(Exception::kValueError);  // 电机ID超出范围
  }
}

/***
 * @brief 推送电机控制消息
 */
void GM6020::PushControlMessage() {
  if (1 <= this->id_ && this->id_ <= 4) {
    this->tx_header_.StdId = 0x1ff;
    this->Transmit(GM6020::tx_buffer_, 8);

  } else if (5 <= this->id_ && this->id_ <= 7) {
    this->tx_header_.StdId = 0x2ff;
    this->Transmit(GM6020::tx_buffer_ + 8, 8);

  } else {
    ThrowException(Exception::kValueError);  // 电机ID超出范围
  }
}

/****************************
 * CLASS M2006
 ****************************/

uint8_t components::motor::M2006::tx_buffer_[16] = {0};

/***
 * @brief 构造函数
 * @param hcan  指向CAN总线对象的指针
 * @param id    电机ID(1~8)
 */
M2006::M2006(CAN_HandleTypeDef *hcan, uint16_t id)
    : DjiMotorBase(hcan, 0x200 + id) {
  if (1 <= id && id <= 8) {
    this->id_ = id;
  }
}

/***
 * @brief 设置电机的输出电流
 * @note  这个函数不会发送控制消息，需要调用PushControlMessage()函数推送
 * @param current   电流值(-10000 ~ 10000)
 */
void M2006::SetCurrent(int16_t current) {
  current = modules::algorithm::utils::absConstrain(current, (int16_t)10000);

  if (1 <= this->id_ && this->id_ <= 8) {
    M2006::tx_buffer_[(this->id_ - 1) * 2] = (current >> 8) & 0xff;
    M2006::tx_buffer_[(this->id_ - 1) * 2 + 1] = current & 0xff;
  } else {
    ThrowException(Exception::kValueError);  // 电机ID超出范围
  }
}

/***
 * @brief 推送电机控制消息
 */
void M2006::PushControlMessage() {
  if (1 <= this->id_ && this->id_ <= 4) {
    this->tx_header_.StdId = 0x200;
    this->Transmit(M2006::tx_buffer_, 8);

  } else if (5 <= this->id_ && this->id_ <= 8) {
    this->tx_header_.StdId = 0x1ff;
    this->Transmit(M2006::tx_buffer_ + 8, 8);

  } else {
    ThrowException(Exception::kValueError);  // 电机ID超出范围
  }
}

/****************************
 * CLASS M3508
 ****************************/

uint8_t components::motor::M3508::tx_buffer_[16] = {0};

/***
 * @brief 构造函数
 * @param can_bus   指向CAN总线对象的指针
 * @param id        电机ID(1~8)
 */
M3508::M3508(CAN_HandleTypeDef *hcan, uint16_t id)
    : DjiMotorBase(hcan, 0x201 + id) {
  if (1 <= id && id <= 8) {
    this->id_ = id;
  } else {
    ThrowException(Exception::kValueError);  // 电机ID超出范围
  }
}

/***
 * @brief 设置电机的输出电流
 * @param current   电流值(-16384 ~ 16384)
 */
void M3508::SetCurrent(int16_t current) {
  current = modules::algorithm::utils::absConstrain(current, (int16_t)16384);

  if (1 <= this->id_ && this->id_ <= 8) {
    M3508::tx_buffer_[(this->id_ - 1) * 2] = (current >> 8) & 0xff;
    M3508::tx_buffer_[(this->id_ - 1) * 2 + 1] = current & 0xff;
  } else {
    ThrowException(Exception::kValueError);
  }
}
void M3508::PushControlMessage() {
  if (1 <= this->id_ && this->id_ <= 4) {
    this->tx_header_.StdId = 0x200;
    this->Transmit(M3508::tx_buffer_, 8);

  } else if (5 <= this->id_ && this->id_ <= 8) {
    this->tx_header_.StdId = 0x1ff;
    this->Transmit(M3508::tx_buffer_ + 8, 8);

  } else {
    ThrowException(Exception::kValueError);  // 电机ID超出范围
  }
}

#endif

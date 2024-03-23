
/**
 * @file  components/motor/dji/dji_motor.cc
 * @brief 大疆电机的类封装
 */

#include "hal_wrapper/hal_config.h"

#ifdef HAL_CAN_MODULE_ENABLED

#include "dji_motor.h"
#include "modules/algorithm/utils/utils.hpp"
#include "modules/exceptions/exceptions.h"

using namespace irobot_ec::components::motor;
using irobot_ec::modules::algorithm::utils::absConstrain;
using irobot_ec::modules::exceptions::Exception;
using irobot_ec::modules::exceptions::ThrowException;

/***************************
 * CLASS DJIMotorBase
 ****************************/

/**
 * @brief 构造函数
 * @param hcan  指向CAN总线对象的指针
 * @param id    电机ID
 */
DjiMotorBase::DjiMotorBase(CAN_HandleTypeDef *hcan, uint16_t rx_std_id)
    : hal::can::CanDeviceBase(hcan, rx_std_id) {
  // 第一次调用构造函数，分配这个电机对象所在CAN总线的发送缓冲区
  if (this->tx_buf_ == nullptr) {
    this->tx_buf_ = new ::std::unordered_map<CAN_HandleTypeDef *,
                                             ::std::array<uint8_t, 16>>(
        {{this->hcan_, {}}});
  }

  // 构造的电机对象所在的CAN总线对应的发送缓冲区还未分配，就给它分配一个
  if (this->tx_buf_->find(this->hcan_) == this->tx_buf_->end()) {
    this->tx_buf_->insert({this->hcan_, {}});
  }
}

/**
 * @brief   电机反馈数据解码回调函数
 */
void DjiMotorBase::RxCallback(hal::can::CanRxMsg *msg) {
  this->encoder_ = (msg->data[0] << 8) | msg->data[1];
  this->rpm_ = (msg->data[2] << 8) | msg->data[3];
  this->current_ = (msg->data[4] << 8) | msg->data[5];
  this->temperature_ = (msg->data[6] << 8) | msg->data[7];
}

/**
 * @brief   获取电机的编码器值
 * @return  编码器值(0~8191 => 0~360°)
 */
uint16_t DjiMotorBase::encoder() const { return this->encoder_; }

/**
 * @brief   获取电机的转速
 * @return  RPM(rad/s)
 */
uint16_t DjiMotorBase::rpm() const { return this->rpm_; }

/**
 * @brief   获取电机的实际电流
 * @return  电流值(无单位)
 */
uint16_t DjiMotorBase::current() const { return this->current_; }

/**
 * @brief   获取电机的温度
 * @return  温度(°C)
 */
uint16_t DjiMotorBase::temperature() const { return this->temperature_; }

/**
 * @brief  设置电机的输出电流，本模板函数留给子类实例化，设定电流范围
 * @tparam current_bound 电流最大值
 * @param  current       设定电流值
 */
template <int16_t current_bound>
void DjiMotorBase::SetCurrentTemplate(int16_t current) {
  current = absConstrain(current, (int16_t)current_bound);

  (*this->tx_buf_)[this->hcan_][(this->id_ - 1) * 2] = (current >> 8) & 0xff;
  (*this->tx_buf_)[this->hcan_][(this->id_ - 1) * 2 + 1] = current & 0xff;
  if (1 <= this->id_ && this->id_ <= 4) {
    this->Transmit((*this->tx_buf_)[this->hcan_].data(), 8);
  } else if (5 <= this->id_ && this->id_ <= 7) {
    this->Transmit((*this->tx_buf_)[this->hcan_].data() + 8, 8);
  }
}

/***************************
 * CLASS GM6020
 ****************************/

/**
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

/**
 * @brief 设置电机的输出电流
 * @param current 电流值(-30000 ~ 30000)
 */
void GM6020::SetCurrent(int16_t current) {
  DjiMotorBase::SetCurrentTemplate<30000>(current);
}

/***************************
 * CLASS M2006
 ****************************/

/**
 * @brief 构造函数
 * @param hcan  指向CAN总线对象的指针
 * @param id    电机ID(1~8)
 */
M2006::M2006(CAN_HandleTypeDef *hcan, uint16_t id)
    : DjiMotorBase(hcan, 0x200 + id) {
  if (1 <= id && id <= 8) {
    this->id_ = id;
  } else {
    ThrowException(Exception::kValueError);  // 电机ID超出范围
  }
}

/**
 * @brief 设置电机的输出电流
 * @param current 电流值(-10000 ~ 10000)
 */
void M2006::SetCurrent(int16_t current) {
  DjiMotorBase::SetCurrentTemplate<10000>(current);
}

/***************************
 * CLASS M3508
 ****************************/

/**
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

/**
 * @brief 设置电机的输出电流
 * @param current 电流值(-16384 ~ 16384)
 */
void M3508::SetCurrent(int16_t current) {
  DjiMotorBase::SetCurrentTemplate<16384>(current);
}

#endif  // HAL_CAN_MODULE_ENABLED


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

::std::array<::std::unordered_map<CAN_HandleTypeDef *, ::std::array<uint8_t, 16>> *, 3> DjiMotorBase::tx_bufs_ = {
    nullptr, nullptr, nullptr};

/**
 * @brief 构造函数
 * @param hcan  指向CAN总线对象的指针
 * @param id    电机ID
 */
DjiMotorBase::DjiMotorBase(uint8_t buffer_index, CAN_HandleTypeDef *hcan, uint16_t rx_std_id)
    : hal::can::CanDeviceBase(hcan, rx_std_id) {
  // 对应电机类型的发送缓冲区组还未创建，就创建一个
  if (DjiMotorBase::tx_bufs_[buffer_index] == nullptr) {
    DjiMotorBase::tx_bufs_[buffer_index] = new ::std::unordered_map<CAN_HandleTypeDef *, ::std::array<uint8_t, 16>>;
  }

  // 构造的电机对象所在的CAN总线对应的发送缓冲区还未分配，就给它分配一个
  if (DjiMotorBase::tx_bufs_[buffer_index]->find(this->hcan_) == DjiMotorBase::tx_bufs_[buffer_index]->end()) {
    DjiMotorBase::tx_bufs_[buffer_index]->insert({this->hcan_, {}});
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
template <int16_t current_bound, uint8_t buffer_index>
void DjiMotorBase::SetCurrentTemplate(int16_t current) {
  current = absConstrain(current, (int16_t)current_bound);

  /**
   * 这段代码的解释：
   * 1. 根据电机类型(buffer_index)找到对应的发送缓冲区组
   * 2. 根据这个电机所属的can总线(this->hcan_)找到对应的发送缓冲区
   * 3. 根据电机ID(this->id_)找到对应的发送缓冲区
   * 4. 将电流值写入发送缓冲区
   * 5. 判断电机ID是1~4还是5~8，发送16个字节的缓冲区的前8个字节或后8个字节
   *
   * @btw DJI I LOVE THIS MOTOR CONTROL PROTOCOL :)
   */
  (*DjiMotorBase::tx_bufs_[buffer_index])[this->hcan_][(this->id_ - 1) * 2] = (current >> 8) & 0xff;
  (*DjiMotorBase::tx_bufs_[buffer_index])[this->hcan_][(this->id_ - 1) * 2 + 1] = current & 0xff;
  if (1 <= this->id_ && this->id_ <= 4) {
    this->Transmit((*DjiMotorBase::tx_bufs_[buffer_index])[this->hcan_].data(), 8);
  } else if (5 <= this->id_ && this->id_ <= 7) {
    this->Transmit((*DjiMotorBase::tx_bufs_[buffer_index])[this->hcan_].data() + 8, 8);
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
GM6020::GM6020(CAN_HandleTypeDef *hcan, uint16_t id) : DjiMotorBase(0, hcan, 0x204 + id) {
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
void GM6020::SetCurrent(int16_t current) { DjiMotorBase::SetCurrentTemplate<30000, 0>(current); }

/***************************
 * CLASS M2006
 ****************************/

/**
 * @brief 构造函数
 * @param hcan  指向CAN总线对象的指针
 * @param id    电机ID(1~8)
 */
M2006::M2006(CAN_HandleTypeDef *hcan, uint16_t id) : DjiMotorBase(1, hcan, 0x200 + id) {
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
void M2006::SetCurrent(int16_t current) { DjiMotorBase::SetCurrentTemplate<10000, 1>(current); }

/***************************
 * CLASS M3508
 ****************************/

/**
 * @brief 构造函数
 * @param can_bus   指向CAN总线对象的指针
 * @param id        电机ID(1~8)
 */
M3508::M3508(CAN_HandleTypeDef *hcan, uint16_t id) : DjiMotorBase(2, hcan, 0x201 + id) {
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
void M3508::SetCurrent(int16_t current) { DjiMotorBase::SetCurrentTemplate<16384, 2>(current); }

#endif  // HAL_CAN_MODULE_ENABLED

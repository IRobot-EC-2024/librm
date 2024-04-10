/**
 * @file  hal_wrapper/can_device.cc
 * @brief CAN外设包装类
 * @todo  这里需要同时支持bxcan和fdcan，两者的API不同，需要重构，去掉所有对HAL库的直接依赖
 * @todo  2024/4/5 第一次重构，将设置CAN过滤器的代码从这里移动到bsp库中
 */

#include "hal.h"
#if defined(HAL_CAN_MODULE_ENABLED)

#include "can_device.h"

#include "bsp/bsp.h"
#include "modules/exception/exception.h"

using namespace irobot_ec::hal::can;
using irobot_ec::modules::exception::Exception;
using irobot_ec::modules::exception::ThrowException;

/***************************
 * CLASS CANDeviceBase
 ****************************/

std::unordered_map<CAN_HandleTypeDef*, std::unordered_map<u32, CanDeviceBase*>> CanDeviceBase::device_map_ = {};

/**
 * @brief 析构函数
 * @brief 从device_map_中注销这个设备
 */
CanDeviceBase::~CanDeviceBase() { CanDeviceBase::device_map_[this->hcan_].erase(this->rx_std_id_); }

/**
 * @brief 构造函数
 * @param hcan      指向CAN外设句柄结构体的指针
 * @param rx_std_id 本设备的rx标准帧ID
 */
CanDeviceBase::CanDeviceBase(CAN_HandleTypeDef* hcan, u32 rx_std_id)
    : hcan_(hcan),
      rx_std_id_(rx_std_id),
      tx_header_{.StdId = 0, .ExtId = 0, .IDE = CAN_ID_STD, .RTR = CAN_RTR_DATA, .DLC = 8} {
  // 检查是否已经初始化过这个CAN总线
  if (CanDeviceBase::device_map_.find(hcan) == CanDeviceBase::device_map_.end()) {
    bsp::BspFactory::GetCan().InitCanFilter(hcan);
    CanDeviceBase::device_map_[hcan] = std::unordered_map<u32, CanDeviceBase*>();
  }

  // 检查这个CAN总线上有没有rx_std_id相同的设备
  if (CanDeviceBase::device_map_[hcan].find(rx_std_id) != CanDeviceBase::device_map_[hcan].end()) {
    ThrowException(Exception::kValueError);  // rx_std_id冲突
  }

  CanDeviceBase::device_map_[hcan][rx_std_id] = this;
}

/**
 * @brief 向CAN总线发送数据
 * @param data 数据指针
 * @param size 数据长度
 */
void CanDeviceBase::Transmit(const u8* data, u32 size) {
  this->tx_header_.DLC = size;
  if (HAL_CAN_AddTxMessage(this->hcan_, &this->tx_header_, const_cast<u8*>(data), &this->tx_mailbox_) != HAL_OK) {
    ThrowException(Exception::kHALError);  // HAL_CAN_AddTxMessage error
  }
}

#endif

/* EOF */

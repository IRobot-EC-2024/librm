
/**
 * @file  bsp/bsp_can.cpp
 * @brief CAN peripheral c++ wrapper implementations
 */

#include "hal_wrapper/hal_config.h"

#ifdef HAL_CAN_MODULE_ENABLED

#include "hal_can.h"
#include "modules/exception/exception.h"

using namespace irobot_ec::hal::can;
using irobot_ec::modules::exception::Exception;
using irobot_ec::modules::exception::ThrowException;

static CanRxMsg rx_msg_buffer;

#ifdef __cplusplus
extern "C" {
#endif

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef* hcan) {
  HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &rx_msg_buffer.header, rx_msg_buffer.data);

  if (CanDeviceBase::device_map_.find(hcan) == CanDeviceBase::device_map_.end()) {
    return;
  }

  if (CanDeviceBase::device_map_[hcan].find(rx_msg_buffer.header.StdId) == CanDeviceBase::device_map_[hcan].end()) {
    return;
  }

  CanDeviceBase::device_map_[hcan][rx_msg_buffer.header.StdId]->RxCallback(&rx_msg_buffer);
}

#ifdef __cplusplus
}
#endif

static void CanFilterInit(CAN_HandleTypeDef* hcan) {
#ifdef STM32F407xx
  CAN_FilterTypeDef can_filter_st;
  can_filter_st.FilterActivation = ENABLE;
  can_filter_st.FilterMode = CAN_FILTERMODE_IDMASK;
  can_filter_st.FilterScale = CAN_FILTERSCALE_32BIT;
  can_filter_st.FilterIdHigh = 0x0000;
  can_filter_st.FilterIdLow = 0x0000;
  can_filter_st.FilterMaskIdHigh = 0x0000;
  can_filter_st.FilterMaskIdLow = 0x0000;
  can_filter_st.FilterFIFOAssignment = CAN_RX_FIFO0;
  if (reinterpret_cast<uint32_t>(hcan->Instance) == 0x40006400) {
    // 如果是CAN1
    can_filter_st.FilterBank = 0;
  } else if (reinterpret_cast<uint32_t>(hcan->Instance) == 0x40006800) {
    // 如果是CAN2
    can_filter_st.SlaveStartFilterBank = 14;
    can_filter_st.FilterBank = 14;
  }
  if (HAL_CAN_ConfigFilter(hcan, &can_filter_st) != HAL_OK) {
    ThrowException(Exception::kHALError);  // HAL_CAN_ConfigFilter error
  }
  if (HAL_CAN_Start(hcan) != HAL_OK) {
    ThrowException(Exception::kHALError);  // HAL_CAN_Start error
  }
  if (HAL_CAN_ActivateNotification(hcan, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK) {
    ThrowException(Exception::kHALError);  // HAL_CAN_ActivateNotification error
  }
#endif
}

/***************************
 * CLASS CANDeviceBase
 ****************************/

std::unordered_map<CAN_HandleTypeDef*, std::unordered_map<uint32_t, CanDeviceBase*>> CanDeviceBase::device_map_ = {};

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
CanDeviceBase::CanDeviceBase(CAN_HandleTypeDef* hcan, uint32_t rx_std_id)
    : hcan_(hcan),
      rx_std_id_(rx_std_id),
      tx_header_{.StdId = rx_std_id, .ExtId = 0, .IDE = CAN_ID_STD, .RTR = CAN_RTR_DATA, .DLC = 8} {
  // 检查是否已经初始化过这个CAN总线
  if (CanDeviceBase::device_map_.find(hcan) == CanDeviceBase::device_map_.end()) {
    CanFilterInit(hcan);
    CanDeviceBase::device_map_[hcan] = std::unordered_map<uint32_t, CanDeviceBase*>();
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
void CanDeviceBase::Transmit(const uint8_t* data, uint32_t size) {
  if (HAL_CAN_AddTxMessage(this->hcan_, &this->tx_header_, const_cast<uint8_t*>(data), &this->tx_mailbox_) != HAL_OK) {
    ThrowException(Exception::kHALError);  // HAL_CAN_AddTxMessage error
  }
}

#endif

/* EOF */

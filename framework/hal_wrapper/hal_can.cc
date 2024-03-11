
/***
 * @file  bsp/bsp_can.cpp
 * @brief CAN peripheral c++ wrapper implementations
 */

#include "hal_wrapper/hal_config.h"

#ifdef HAL_CAN_MODULE_ENABLED

#include "hal_can.h"
#include "modules/exceptions/exceptions.h"

using namespace hal::can;
using modules::exceptions::Exception;
using modules::exceptions::ThrowException;

can_rx_msg_t rx_msg_buffer;

#ifdef __cplusplus
extern "C" {
#endif

#ifdef HAL_CAN_MODULE_ENABLED

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan) {
  HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &rx_msg_buffer.header,
                       rx_msg_buffer.data);

  if (CANDeviceBase::device_map_.find(hcan) ==
      CANDeviceBase::device_map_.end()) {
    return;
  }

  if (CANDeviceBase::device_map_[hcan].find(rx_msg_buffer.header.StdId) ==
      CANDeviceBase::device_map_[hcan].end()) {
    return;
  }

  CANDeviceBase::device_map_[hcan][rx_msg_buffer.header.StdId]->rxCallback(
      &rx_msg_buffer);
}

#endif

#ifdef __cplusplus
}
#endif

void canFilterInit() {
  CAN_FilterTypeDef can_filter_st;
  can_filter_st.FilterActivation = ENABLE;
  can_filter_st.FilterMode = CAN_FILTERMODE_IDMASK;
  can_filter_st.FilterScale = CAN_FILTERSCALE_32BIT;
  can_filter_st.FilterIdHigh = 0x0000;
  can_filter_st.FilterIdLow = 0x0000;
  can_filter_st.FilterMaskIdHigh = 0x0000;
  can_filter_st.FilterMaskIdLow = 0x0000;
  can_filter_st.FilterBank = 0;
  can_filter_st.FilterFIFOAssignment = CAN_RX_FIFO0;
  HAL_CAN_ConfigFilter(&hcan1, &can_filter_st);
  HAL_CAN_Start(&hcan1);
  HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING);

  //        can_filter_st.SlaveStartFilterBank = 14;
  //        can_filter_st.FilterBank = 14;
  //        HAL_CAN_ConfigFilter(&hcan2, &can_filter_st);
  //        HAL_CAN_Start(&hcan2);
  //        HAL_CAN_ActivateNotification(&hcan2, CAN_IT_RX_FIFO0_MSG_PENDING);
}

/****************************
 * CLASS CANDeviceBase
 ****************************/

std::unordered_map<CAN_HandleTypeDef *,
                   std::unordered_map<uint32_t, CANDeviceBase *>>
    CANDeviceBase::device_map_ = {};

/***
 * @brief Destructor
 * @brief Remove this device from the device map
 */
CANDeviceBase::~CANDeviceBase() {
  CANDeviceBase::device_map_[this->hcan_].erase(this->rx_std_id_);
}

/***
 * @brief Constructor
 * @param can_bus Pointer to CAN bus object
 * @param rx_std_id Standard ID of this device's RX message
 */
CANDeviceBase::CANDeviceBase(CAN_HandleTypeDef *hcan, uint32_t rx_std_id)
    : hcan_(hcan),
      rx_std_id_(rx_std_id),
      tx_header_{.StdId = rx_std_id,
                 .ExtId = 0,
                 .IDE = CAN_ID_STD,
                 .RTR = CAN_RTR_DATA,
                 .DLC = 8} {
  // Check if hcan key exists in the map
  if (CANDeviceBase::device_map_.find(hcan) ==
      CANDeviceBase::device_map_.end()) {
    CANDeviceBase::device_map_[hcan] =
        std::unordered_map<uint32_t, CANDeviceBase *>();
  }

  // Check rx_std_id conflict
  if (CANDeviceBase::device_map_[hcan].find(rx_std_id) !=
      CANDeviceBase::device_map_[hcan].end()) {
    ThrowException(Exception::kValueError);  // rx_std_id conflict
  }

  CANDeviceBase::device_map_[hcan][rx_std_id] = this;
}

/***
 * @brief Transmit data on the CAN bus attached to
 * @param data Pointer to TX buffer
 * @param size Bytes to transmit
 */
void CANDeviceBase::transmit(const uint8_t *data, uint32_t size) {
  if (HAL_CAN_AddTxMessage(this->hcan_, &this->tx_header_,
                           const_cast<uint8_t *>(data),
                           &this->tx_mailbox_) != HAL_OK) {
    ThrowException(Exception::kHALError);  // HAL_CAN_AddTxMessage error
  }
}

#endif

/* EOF */

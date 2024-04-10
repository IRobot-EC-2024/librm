/**
 * @file  bsp/dji_devboard_c/bsp_can.cc
 * @brief 对大疆c板CAN外设的一些操作集合
 * @todo  see: src/hal_wrapper/can_device.h
 */

#include "hal/hal.h"
#if defined(HAL_CAN_MODULE_ENABLED) && defined(STM32F407xx)

#include "bsp_can.h"

#include "hal/can_device.h"
#include "modules/exception/exception.h"

using irobot_ec::hal::can::CanDeviceBase;
using irobot_ec::modules::exception::Exception;
using irobot_ec::modules::exception::ThrowException;

static irobot_ec::bsp::CanRxMsg rx_msg_buffer;

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

namespace irobot_ec::bsp::dji_devboard_c {

void Can::InitCanFilter(CAN_HandleTypeDef* hcan, u16 id, u16 mask) {
  CAN_FilterTypeDef can_filter_st;
  can_filter_st.FilterActivation = ENABLE;
  can_filter_st.FilterMode = CAN_FILTERMODE_IDMASK;
  can_filter_st.FilterScale = CAN_FILTERSCALE_32BIT;
  can_filter_st.FilterIdHigh = id >> 8;
  can_filter_st.FilterIdLow = id;
  can_filter_st.FilterMaskIdHigh = mask >> 8;
  can_filter_st.FilterMaskIdLow = mask;
  can_filter_st.FilterFIFOAssignment = CAN_RX_FIFO0;
  if (reinterpret_cast<u32>(hcan->Instance) == 0x40006400) {
    // 如果是CAN1
    can_filter_st.FilterBank = 0;
  } else if (reinterpret_cast<u32>(hcan->Instance) == 0x40006800) {
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
}

Can& Can::GetInstance() {
  static Can instance;
  return instance;
}

}  // namespace irobot_ec::bsp::dji_devboard_c

#endif
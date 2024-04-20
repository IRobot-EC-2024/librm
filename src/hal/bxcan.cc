/**
 * @file  hal/bxcan.cc
 * @brief bxCAN类库
 */

#include "hal/hal.h"
#if defined(HAL_CAN_MODULE_ENABLED)
#if defined(USE_HAL_CAN_REGISTER_CALLBACKS)
#if (USE_HAL_CAN_REGISTER_CALLBACKS != 1u)
#error "CAN register callback must be enabled!"
#endif
#endif

#include "bxcan.h"

#include <functional>

#include "hal/can_device.hpp"
#include "modules/exception/exception.h"

using irobot_ec::hal::CanDeviceBase;
using irobot_ec::modules::exception::Exception;
using irobot_ec::modules::exception::ThrowException;

/**
 * @brief  把std::function转换为函数指针
 * @param  fn   要转换的函数
 * @return      转换后的函数指针
 * @note
 * 背景：因为要用面向对象的方式对外设进行封装，所以回调函数必须存在于类内。但是存在于类内就意味着这个回调函数多了一个this参数，
 * 而HAL库要求的回调函数并没有这个this参数。通过std::bind，可以生成一个参数列表里没有this指针的std::function对象，而std::function
 * 并不能直接强转成函数指针。借助这个函数，可以把std::function对象转换成函数指针。然后就可以把这个类内的回调函数传给HAL库了。
 */
static pCAN_CallbackTypeDef StdFunctionToCallbackFunctionPtr(std::function<void()> fn) {
  static auto fn_v = std::move(fn);
  return [](CAN_HandleTypeDef *handle) { fn_v(); };
}

namespace irobot_ec::hal {

/**
 * @param hcan HAL库的CAN_HandleTypeDef
 */
BxCan::BxCan(CAN_HandleTypeDef &hcan) : hcan_(&hcan) {
  HAL_CAN_RegisterCallback(this->hcan_, HAL_CAN_RX_FIFO0_MSG_PENDING_CB_ID,
                           StdFunctionToCallbackFunctionPtr(std::bind(&BxCan::Fifo0MsgPendingCallback, this)));
  this->SetFilter(0, 0);
  this->Begin();
}

/**
 * @brief 设置过滤器
 * @param id
 * @param mask
 */
void BxCan::SetFilter(u16 id, u16 mask) {
  CAN_FilterTypeDef can_filter_st;
  can_filter_st.FilterActivation = ENABLE;
  can_filter_st.FilterMode = CAN_FILTERMODE_IDMASK;
  can_filter_st.FilterScale = CAN_FILTERSCALE_32BIT;
  can_filter_st.FilterIdHigh = id >> 8;
  can_filter_st.FilterIdLow = id;
  can_filter_st.FilterMaskIdHigh = mask >> 8;
  can_filter_st.FilterMaskIdLow = mask;
  can_filter_st.FilterFIFOAssignment = CAN_RX_FIFO0;
  if (reinterpret_cast<u32>(this->hcan_->Instance) == CAN1_BASE) {
    // 如果是CAN1
    can_filter_st.FilterBank = 0;
  } else if (reinterpret_cast<u32>(this->hcan_->Instance) == CAN2_BASE) {
    // 如果是CAN2
    can_filter_st.SlaveStartFilterBank = 14;
    can_filter_st.FilterBank = 14;
  }
  if (HAL_CAN_ConfigFilter(this->hcan_, &can_filter_st) != HAL_OK) {
    ThrowException(Exception::kHALError);  // HAL_CAN_ConfigFilter error
  }
}

/**
 * @brief 向总线上发送数据
 * @param id    数据帧ID
 * @param data  数据指针
 * @param size  数据长度
 */
void BxCan::Write(u16 id, const u8 *data, usize size) {
  static CAN_TxHeaderTypeDef tx_header;
  tx_header.StdId = id;
  tx_header.ExtId = 0;
  tx_header.IDE = CAN_ID_STD;
  tx_header.RTR = CAN_RTR_DATA;
  tx_header.DLC = size;
  if (HAL_CAN_AddTxMessage(this->hcan_, &tx_header, const_cast<u8 *>(data), &this->tx_mailbox_) != HAL_OK) {
    ThrowException(Exception::kHALError);  // HAL_CAN_AddTxMessage error
  }
}

/**
 * @brief 启动CAN外设
 */
void BxCan::Begin() {
  if (HAL_CAN_Start(this->hcan_) != HAL_OK) {
    ThrowException(Exception::kHALError);  // HAL_CAN_Start error
  }
  if (HAL_CAN_ActivateNotification(this->hcan_, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK) {
    ThrowException(Exception::kHALError);  // HAL_CAN_ActivateNotification error
  }
}

/**
 * @brief 停止CAN外设
 */
void BxCan::Stop() {
  if (HAL_CAN_Stop(this->hcan_) != HAL_OK) {
    ThrowException(Exception::kHALError);  // HAL_CAN_Stop error
  };
}

/**
 * @brief FIFO0消息挂起回调函数
 * @note  这个函数会被HAL库调用
 */
void BxCan::Fifo0MsgPendingCallback() {
  static CAN_RxHeaderTypeDef rx_header;
  HAL_CAN_GetRxMessage(this->hcan_, CAN_RX_FIFO0, &rx_header, this->rx_msg_buffer_.data);
  if (this->device_list_.find(rx_header.StdId) == this->device_list_.end()) {
    return;
  }
  this->rx_msg_buffer_.rx_std_id = rx_header.StdId;
  this->rx_msg_buffer_.dlc = rx_header.DLC;
  this->device_list_[rx_header.StdId]->RxCallback(&this->rx_msg_buffer_);
}

}  // namespace irobot_ec::hal

#endif
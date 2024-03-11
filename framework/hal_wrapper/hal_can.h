
/***
 * @file  bsp/bsp_can.h
 * @brief CAN device c++ wrapper
 */

#pragma once

#ifndef EC_LIB_HAL_WRAPPER_HAL_CAN_H
#define EC_LIB_HAL_WRAPPER_HAL_CAN_H

#include "hal_wrapper/hal_config.h"

#ifdef HAL_CAN_MODULE_ENABLED

#include "can.h"

#include <unordered_map>

namespace hal::can {

/***
 * @brief CAN message data pack
 */
typedef struct {
  uint8_t data[8];
  CAN_RxHeaderTypeDef header;
} can_rx_msg_t;

/***
 * @brief CAN device base class
 */
class CANDeviceBase {

  friend void::HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan);

 public:
  ~CANDeviceBase();
  CANDeviceBase(CAN_HandleTypeDef *hcan, uint32_t rx_std_id);

  virtual void rxCallback(can_rx_msg_t *msg) = 0;     // Called from CANBus::rxCallback to invoke message handling
  void transmit(const uint8_t *data, uint32_t size);

 protected:
  uint32_t rx_std_id_;             // rx standard id
  uint32_t tx_mailbox_;            // tx mailbox
  CAN_TxHeaderTypeDef tx_header_;             // tx header
  CAN_HandleTypeDef *hcan_;                  // CAN peripheral handle

 private:
  // <hcan, <rx_std_id, device>>
  static std::unordered_map<CAN_HandleTypeDef *, std::unordered_map<uint32_t, CANDeviceBase *>> device_map_;
};

}   // namespace bsp::CAN


#endif

#endif // EC_LIB_HAL_WRAPPER_HAL_CAN_H

/* EOF */

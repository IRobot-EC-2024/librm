
/**
 * @file  bsp/bsp_can.h
 * @brief CAN device c++ wrapper
 */

#ifndef EC_LIB_HAL_WRAPPER_HAL_CAN_H
#define EC_LIB_HAL_WRAPPER_HAL_CAN_H

#include "hal_wrapper/hal_config.h"

#ifdef HAL_CAN_MODULE_ENABLED

#include <unordered_map>

#include "can.h"

namespace irobot_ec::hal::can {

/**
 * @brief CAN message data pack
 */
struct CanRxMsg {
  uint8_t data[8];
  CAN_RxHeaderTypeDef header;
};

/**
 * @brief CAN device base class
 */
class CanDeviceBase {
  friend void ::HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan);

 public:
  ~CanDeviceBase();
  CanDeviceBase(CAN_HandleTypeDef *hcan, uint32_t rx_std_id);

  virtual void RxCallback(CanRxMsg *msg) = 0;

  void Transmit(const uint8_t *data, uint32_t size);

 protected:
  uint32_t rx_std_id_;             // rx standard id
  uint32_t tx_mailbox_;            // tx mailbox
  CAN_TxHeaderTypeDef tx_header_;  // tx header
  CAN_HandleTypeDef *hcan_;        // CAN peripheral handle

 private:
  // <hcan, <rx_std_id, device>>
  static std::unordered_map<CAN_HandleTypeDef *,
                            std::unordered_map<uint32_t, CanDeviceBase *>>
      device_map_;
};

}  // namespace irobot_ec::hal::can

#endif

#endif  // EC_LIB_HAL_WRAPPER_HAL_CAN_H

/* EOF */

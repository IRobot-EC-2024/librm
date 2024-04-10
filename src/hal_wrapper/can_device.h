
/**
 * @file  hal_wrapper/can_device.h
 * @brief CAN外设包装类
 * @todo  这里需要同时支持bxcan和fdcan，两者的API不同，需要重构，去掉所有对HAL库的直接依赖
 * @todo  2024/4/5 第一次重构，将设置CAN过滤器的代码从这里移动到bsp库中
 */

#ifndef EC_LIB_HAL_WRAPPER_HAL_CAN_H
#define EC_LIB_HAL_WRAPPER_HAL_CAN_H

#include "hal_wrapper/hal.h"

#ifdef HAL_CAN_MODULE_ENABLED

#include <unordered_map>
#include "can.h"

#include "bsp/interface/bsp_can_interface.h"

namespace irobot_ec::hal::can {

/**
 * @brief CAN设备的基类
 */
class CanDeviceBase {
  friend void ::HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan);

 public:
  ~CanDeviceBase();
  CanDeviceBase() = default;
  CanDeviceBase(CAN_HandleTypeDef *hcan, u32 rx_std_id);

  virtual void RxCallback(bsp::CanRxMsg *msg) = 0;

  void Transmit(const u8 *data, u32 size);

 protected:
  u32 rx_std_id_;             // rx standard id
  u32 tx_mailbox_;            // tx mailbox
  CAN_TxHeaderTypeDef tx_header_;  // tx header
  CAN_HandleTypeDef *hcan_;        // CAN peripheral handle

 private:
  // <hcan, <rx_std_id, device>>
  static std::unordered_map<CAN_HandleTypeDef *, std::unordered_map<u32, CanDeviceBase *>> device_map_;
};

}  // namespace irobot_ec::hal::can

#endif

#endif  // EC_LIB_HAL_WRAPPER_HAL_CAN_H

/* EOF */

/**
 * @file  bsp/common/can_device.h
 * @brief CAN设备包装
 */

#ifndef EC_LIB_HAL_WRAPPER_HAL_CAN_H
#define EC_LIB_HAL_WRAPPER_HAL_CAN_H

#include "hal/hal.h"
#if defined(USE_HAL_CAN_REGISTER_CALLBACKS)
#if (USE_HAL_CAN_REGISTER_CALLBACKS != 1u)
#error "CAN register callback must be enabled!"
#endif
#endif
#if defined(HAL_CAN_MODULE_ENABLED)

#include <unordered_map>

#include "bsp/interface/bsp_can_interface.h"

namespace irobot_ec::bsp {

/**
 * @brief CAN设备的基类
 */
class CanDeviceBase {
 public:
  virtual ~CanDeviceBase() = default;
  CanDeviceBase() = delete;
  CanDeviceBase(CanBase &can, u32 rx_std_id);

  // 禁止拷贝构造
  CanDeviceBase(const CanDeviceBase &) = delete;
  CanDeviceBase &operator=(const CanDeviceBase &) = delete;

  virtual void RxCallback(const bsp::CanRxMsg *msg) = 0;
  CanBase &can();

 protected:
  u32 rx_std_id_;  // 这个设备的rx消息标准帧id
  CanBase *can_;
};

}  // namespace irobot_ec::bsp

#endif

#endif  // EC_LIB_HAL_WRAPPER_HAL_CAN_H

/**
 * @file  device/can_device.h
 * @brief CAN设备抽象类
 */

#ifndef EC_LIB_HAL_CAN_DEVICE_HPP
#define EC_LIB_HAL_CAN_DEVICE_HPP

#include "hal/hal.h"
#if defined(HAL_CAN_MODULE_ENABLED) || defined(HAL_FDCAN_MODULE_ENABLED)

#include "hal/can_interface.h"

namespace irobot_ec::device {

/**
 * @brief CAN设备的基类
 */
class CanDeviceBase {
 public:
  virtual ~CanDeviceBase() = default;

  template <typename... IdList>
  explicit CanDeviceBase(hal::CanBase &can, IdList... rx_std_ids);

  // 禁止拷贝构造
  CanDeviceBase(const CanDeviceBase &) = delete;
  CanDeviceBase &operator=(const CanDeviceBase &) = delete;

  virtual void RxCallback(const hal::CanRxMsg *msg) = 0;

 protected:
  hal::CanBase *can_;
};

/**
 * @param can        CAN外设对象
 * @param rx_std_ids 这个设备的rx消息标准帧id列表
 */
template <typename... IdList>
CanDeviceBase::CanDeviceBase(hal::CanBase &can, IdList... rx_std_ids) : can_(&can) {
  (can.RegisterDevice(*this, rx_std_ids), ...);
}

}  // namespace irobot_ec::device

#endif

#endif  // EC_LIB_HAL_CAN_DEVICE_HPP

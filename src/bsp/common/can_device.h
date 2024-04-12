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
#include <initializer_list>

#include "bsp/interface/bsp_can_interface.h"
#include "modules/exception/exception.h"

namespace irobot_ec::bsp {

/**
 * @brief CAN设备的基类
 */
class CanDeviceBase {
 public:
  virtual ~CanDeviceBase() = default;
  CanDeviceBase() = delete;
  template <typename... IdList>
  explicit CanDeviceBase(CanBase &can, IdList... rx_std_ids);

  // 禁止拷贝构造
  CanDeviceBase(const CanDeviceBase &) = delete;
  CanDeviceBase &operator=(const CanDeviceBase &) = delete;

  virtual void RxCallback(const bsp::CanRxMsg *msg) = 0;
  CanBase &can();

 protected:
  CanBase *can_;
};

/**
 * @param can        CAN外设对象
 * @param rx_std_ids 这个设备的rx消息标准帧id列表
 */
template <typename... IdList>
CanDeviceBase::CanDeviceBase(CanBase &can, IdList... rx_std_ids) : can_(&can) {
  static auto loop = [&can, this](auto id) {
    if (can.device_list_.find(id) != can.device_list_.end()) {
      modules::exception::ThrowException(modules::exception::Exception::kValueError);
    }
    can.device_list_[id] = this;
  };
  (loop(rx_std_ids), ...);
}

}  // namespace irobot_ec::bsp

#endif

#endif  // EC_LIB_HAL_WRAPPER_HAL_CAN_H
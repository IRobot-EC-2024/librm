/**
 * @file  bsp/common/can_device.cc
 * @brief CAN设备包装
 */

#include "hal/hal.h"
#if defined(USE_HAL_CAN_REGISTER_CALLBACKS)
#if (USE_HAL_CAN_REGISTER_CALLBACKS != 1u)
#error "CAN register callback must be enabled!"
#endif
#endif
#if defined(HAL_CAN_MODULE_ENABLED)

#include "can_device.h"

#include "modules/exception/exception.h"

namespace irobot_ec::bsp {

using modules::exception::Exception;
using modules::exception::ThrowException;

/**
 * @return 这个设备所在的CAN总线对象
 */
CanBase &CanDeviceBase::can() { return *this->can_; }

/**
 * @param can       CAN外设对象
 * @param rx_std_id 这个设备的rx消息标准帧id
 */
CanDeviceBase::CanDeviceBase(CanBase &can, u32 rx_std_id) : can_(&can), rx_std_id_(rx_std_id) {
  if (can.device_list_.find(rx_std_id) != can.device_list_.end()) {
    ThrowException(Exception::kValueError);
  }
  can.device_list_[rx_std_id] = this;
}

}  // namespace irobot_ec::bsp

#endif

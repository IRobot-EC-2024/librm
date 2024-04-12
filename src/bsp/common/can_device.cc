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

namespace irobot_ec::bsp {

/**
 * @return 这个设备所在的CAN总线对象
 */
CanBase &CanDeviceBase::can() { return *this->can_; }

}  // namespace irobot_ec::bsp

#endif

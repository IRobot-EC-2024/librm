/**
 * @file  hal/can.h
 * @brief 根据HAL库的宏定义决定Can是bxCAN还是fdCAN，并且在interface/can_interface.h里提供一个接口类CanBase实现多态
 */

#ifndef EC_LIB_HAL_CAN_H
#define EC_LIB_HAL_CAN_H

#include "bxcan.h"
#include "fdcan.h"
#include "can_interface.h"
#include "can_device.hpp"

namespace irobot_ec::hal {
#if defined(HAL_CAN_MODULE_ENABLED)
using Can = BxCan;
#elif defined(HAL_FDCAN_MODULE_ENABLED)
using Can = FdCan;  // TODO: 实现FdCan类
#endif
}  // namespace irobot_ec::hal

#endif  // EC_LIB_HAL_CAN_H

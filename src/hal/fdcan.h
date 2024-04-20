/**
 * @file  hal/fdcan.h
 * @brief fdCAN类库
 * @todo  未完成
 */

#ifndef EC_LIB_HAL_FDCAN_H
#define EC_LIB_HAL_FDCAN_H

#include "hal.h"
#if defined(HAL_FDCAN_MODULE_ENABLED)

#include "can_device.hpp"
#include "can_interface.h"

namespace irobot_ec::hal {

class FdCan : public CanBase {};

}  // namespace irobot_ec::hal

#endif

#endif  // EC_LIB_HAL_FDCAN_H
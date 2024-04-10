/**
 * @file  components/rc/dr16.h
 * @brief DR16接收机
 * @todo  未完成
 */

#ifndef EC_LIB_COMPONENTS_DR16_H
#define EC_LIB_COMPONENTS_DR16_H

#include "hal/hal.h"
#if defined(HAL_UART_MODULE_ENABLED)

namespace irobot_ec::components {

typedef enum {
  RC_SWITCH_UP = 1,
  RC_SWITCH_DOWN,
  RC_SWITCH_MID,
} rc_switch_e;

}  // namespace irobot_ec::components

#endif

#endif  // EC_LIB_COMPONENTS_DR16_H

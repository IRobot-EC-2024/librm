
#ifndef EC_LIB_BSP_INTERFACE_BSP_CAN_INTERFACE_H_
#define EC_LIB_BSP_INTERFACE_BSP_CAN_INTERFACE_H_

#include "hal_wrapper/hal_config.h"
#if defined(HAL_CAN_MODULE_ENABLED)

#include "modules/typedefs.h"

namespace irobot_ec::bsp {

struct CanRxMsg {
  uint8_t data[8];
  CAN_RxHeaderTypeDef header;
};

class CanInterface {
 public:
  virtual void Transmit(uint32_t id, uint8_t *data, uint8_t len) = 0;
  virtual void InterruptCallback() = 0;
};

}  // namespace irobot_ec::bsp

#endif

#endif  // EC_LIB_BSP_INTERFACE_BSP_CAN_INTERFACE_H_

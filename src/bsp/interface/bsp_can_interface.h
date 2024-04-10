/**
 * @file    bsp/interface/bsp_can_interface.h
 * @brief   CAN外设接口
 */

#ifndef EC_LIB_BSP_INTERFACE_BSP_CAN_INTERFACE_H_
#define EC_LIB_BSP_INTERFACE_BSP_CAN_INTERFACE_H_

#include "hal/hal.h"
#if defined(HAL_CAN_MODULE_ENABLED)

#include "can.h"

#include "modules/typedefs.h"

namespace irobot_ec::bsp {

struct CanRxMsg {
  u8 data[8];
  CAN_RxHeaderTypeDef header;
};

class CanInterface {
 public:
  virtual ~CanInterface() = default;

  virtual void InitCanFilter(CAN_HandleTypeDef *hcan, u16 id = 0, u16 mask = 0) = 0;
};

}  // namespace irobot_ec::bsp

#endif

#endif  // EC_LIB_BSP_INTERFACE_BSP_CAN_INTERFACE_H_

/**
 * @file    bsp/interface/bsp_can_interface.h
 * @brief   CAN外设接口
 */

#ifndef EC_LIB_BSP_INTERFACE_BSP_CAN_INTERFACE_H_
#define EC_LIB_BSP_INTERFACE_BSP_CAN_INTERFACE_H_

#include "hal/hal.h"
#if defined(HAL_CAN_MODULE_ENABLED)

#include "modules/typedefs.h"

namespace irobot_ec::bsp {

struct CanRxMsg {
  u8 data[8];
  u32 rx_std_id;
  u32 dlc;
};

class CanInterface {
 public:
  virtual ~CanInterface() = default;

  virtual void Write(u16 id, const u8 *data, usize size) = 0;
  virtual void SetFilter(u16 id, u16 mask) = 0;
};

}  // namespace irobot_ec::bsp

#endif

#endif  // EC_LIB_BSP_INTERFACE_BSP_CAN_INTERFACE_H_

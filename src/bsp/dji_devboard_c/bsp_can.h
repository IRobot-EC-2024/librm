/**
 * @file  bsp/dji_devboard_c/bsp_can.h
 * @brief 对大疆c板CAN外设的一些操作集合
 * @todo  see: src/hal_wrapper/can_device.h
 */

#ifndef EC_LIB_BSP_DJI_DEVBOARD_C_BSP_CAN_H_
#define EC_LIB_BSP_DJI_DEVBOARD_C_BSP_CAN_H_

#include "bsp/interface/bsp_can_interface.h"

namespace irobot_ec::bsp::dji_devboard_c {

/**
 * @brief 对CAN外设的一些操作集合
 */
class Can : public CanInterface {
 public:
  Can(const Can &) = delete;
  Can &operator=(const Can &) = delete;

  void InitCanFilter(CAN_HandleTypeDef *hcan, uint16_t id, uint16_t mask) override;

  static Can &GetInstance();

 private:
  Can() = default;
  ~Can() override = default;
};

}  // namespace irobot_ec::bsp::dji_devboard_c

#endif  // EC_LIB_BSP_DJI_DEVBOARD_C_BSP_CAN_H_

/**
 * @file  bsp/dji_devboard_c/bsp_can.h
 * @brief bxCAN类库
 */

#ifndef EC_LIB_BSP_DJI_DEVBOARD_C_BSP_CAN_H_
#define EC_LIB_BSP_DJI_DEVBOARD_C_BSP_CAN_H_

#include "hal/hal.h"
#if defined(USE_HAL_CAN_REGISTER_CALLBACKS)
#if (USE_HAL_CAN_REGISTER_CALLBACKS != 1u)
#error "CAN register callback must be enabled!"
#endif
#endif
#if defined(HAL_CAN_MODULE_ENABLED) && defined(STM32F407xx)

#include <unordered_map>

#include "bsp/common/can_device.hpp"
#include "bsp/interface/bsp_can_interface.h"

namespace irobot_ec::bsp::dji_devboard_c {

/**
 * @brief bxCAN类库
 */
class Can final : public CanBase {
 public:
  explicit Can(CAN_HandleTypeDef &hcan);
  Can() = delete;
  ~Can() override = default;

  // 禁止拷贝构造
  Can(const Can &) = delete;
  Can &operator=(const Can &) = delete;

  void SetFilter(u16 id, u16 mask) override;
  void Write(u16 id, const u8 *data, usize size) override;
  void Begin() override;
  void Stop() override;

 private:
  void Fifo0MsgPendingCallback();

  u32 tx_mailbox_{0};
  CanRxMsg rx_msg_buffer_{};
  CAN_HandleTypeDef *hcan_{nullptr};
};

}  // namespace irobot_ec::bsp::dji_devboard_c

#endif

#endif  // EC_LIB_BSP_DJI_DEVBOARD_C_BSP_CAN_H_

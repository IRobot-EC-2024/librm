/**
 * @file  bsp/dji_devboard_c/bsp_can.h
 * @brief CAN类库
 * @todo  see: src/hal_wrapper/can_device.h
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

#include "bsp/interface/bsp_can_interface.h"

namespace irobot_ec::bsp {

class CanDeviceBase;

namespace dji_devboard_c {

/**
 * @brief bxCAN类库
 */
class Can : public CanInterface {
 public:
  explicit Can(CAN_HandleTypeDef &hcan);
  Can() = delete;
  ~Can() override = default;

  // 禁止拷贝构造
  Can(const Can &) = delete;
  Can &operator=(const Can &) = delete;

  void SetFilter(u16 id, u16 mask) override;
  void Write(u16 id, const u8 *data, usize size) override;
  void Begin();
  void Stop();

 private:
  void FiFo0MsgPendingCallback();

  u32 tx_mailbox_{0};
  CanRxMsg rx_msg_buffer_{};
  CAN_HandleTypeDef *hcan_{nullptr};
  std::unordered_map<u32, CanDeviceBase *> device_list_;  // <rx_std_id, device>
};

}  // namespace dji_devboard_c
}  // namespace irobot_ec::bsp

#endif

#endif  // EC_LIB_BSP_DJI_DEVBOARD_C_BSP_CAN_H_

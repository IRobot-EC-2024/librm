/**
 * @file  hal/bxcan.h
 * @brief bxCAN类库
 */

#ifndef EC_LIB_HAL_BXCAN_H
#define EC_LIB_HAL_BXCAN_H

#include "hal.h"
#if defined(HAL_CAN_MODULE_ENABLED)

#include <unordered_map>

#include "can_interface.h"
#include "device/can_device.hpp"

namespace irobot_ec::hal {

/**
 * @brief bxCAN类库
 */
class BxCan final : public CanBase {
 public:
  explicit BxCan(CAN_HandleTypeDef &hcan);
  BxCan() = delete;
  ~BxCan() override = default;

  // 禁止拷贝构造
  BxCan(const BxCan &) = delete;
  BxCan &operator=(const BxCan &) = delete;

  void SetFilter(u16 id, u16 mask) override;
  void Write(u16 id, const u8 *data, usize size) override;
  void Begin() override;
  void Stop() override;

 private:
  void RegisterDevice(device::CanDeviceBase &device, u32 rx_stdid) override;
  void Fifo0MsgPendingCallback();

  u32 tx_mailbox_{0};
  CanRxMsg rx_msg_buffer_{};
  CAN_HandleTypeDef *hcan_{nullptr};
  std::unordered_map<u16, device::CanDeviceBase *> device_list_{};  // <rx_stdid, device>
};

}  // namespace irobot_ec::hal

#endif

#endif  // EC_LIB_HAL_BXCAN_H

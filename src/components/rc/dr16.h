/**
 * @file  components/rc/dr16.h
 * @brief DR16接收机
 */

#ifndef EC_LIB_COMPONENTS_DR16_H
#define EC_LIB_COMPONENTS_DR16_H

#include "hal/hal.h"
#if defined(HAL_UART_MODULE_ENABLED)

#include <vector>
#include <functional>

#include "modules/typedefs.h"
#include "hal/uart.h"

namespace irobot_ec::components {

/**
 * @brief 遥控器拨杆位置
 */
enum class RcSwitchState : usize {
  kUp = 1u,
  kDown,
  kMid,
};

/**
 * @brief 键盘按键
 */
enum class RcKey : u16 {
  kW = 1u << 0,
  kS = 1u << 1,
  kA = 1u << 2,
  kD = 1u << 3,
  kShift = 1u << 4,
  kCtrl = 1u << 5,
  kQ = 1u << 6,
  kE = 1u << 7,
  kR = 1u << 8,
  kF = 1u << 9,
  kG = 1u << 10,
  kZ = 1u << 11,
  kX = 1u << 12,
  kC = 1u << 13,
  kV = 1u << 14,
  kB = 1u << 15,
};

/**
 * @brief DR16接收机
 */
class DR16 {
 public:
  DR16() = delete;
  explicit DR16(hal::Uart &uart);

  void StartReceive();
  void RxCallback(const std::vector<u8> &data, u16 rx_len);

  [[nodiscard]] i16 left_x() const;
  [[nodiscard]] i16 left_y() const;
  [[nodiscard]] i16 right_x() const;
  [[nodiscard]] i16 right_y() const;
  [[nodiscard]] i16 dial() const;
  [[nodiscard]] RcSwitchState switch_l() const;
  [[nodiscard]] RcSwitchState switch_r() const;
  [[nodiscard]] bool key(RcKey key) const;

 private:
  hal::Uart *uart_;

  i16 axes_[5];                // [0]: right_x, [1]: right_y, [2]: left_x, [3]: left_y, [4]: dial
  i16 mouse_[3];               // [0]: x, [1]: y, [2]: z
  bool mouse_button_[2];       // [0]: left, [1]: right
  RcSwitchState switches_[2];  // [0]: right, [1]: left
  u16 keyboard_key_;           // 每一位代表一个键，0为未按下，1为按下
};

}  // namespace irobot_ec::components

#endif

#endif  // EC_LIB_COMPONENTS_DR16_H

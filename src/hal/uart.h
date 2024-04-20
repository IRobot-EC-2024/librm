/**
 * @file  hal/uart.h
 * @brief UART类库
 */

#ifndef EC_LIB_BSP_COMMON_BSP_UART_H
#define EC_LIB_BSP_COMMON_BSP_UART_H

#include "hal.h"
#if defined(USE_HAL_UART_REGISTER_CALLBACKS)
#if (USE_HAL_UART_REGISTER_CALLBACKS != 1u)
#error "UART register callback must be enabled!"
#endif
#endif
#if defined(HAL_UART_MODULE_ENABLED)

#include <vector>
#include <functional>

#include "modules/typedefs.h"

namespace irobot_ec::hal {

using UartCallbackFunction = std::function<void(const std::vector<u8> &, u16)>;

enum class UartMode {
  kNormal,
  kInterrupt,
#if defined(HAL_DMA_MODULE_ENABLED)
  kDma,
#endif
};

/**
 * @brief UART类
 */
class Uart {
 public:
  Uart(UART_HandleTypeDef &huart, usize rx_buffer_size, UartMode tx_mode = UartMode::kNormal,
       UartMode rx_mode = UartMode::kNormal);

  void Write(const u8 *data, usize size);
  void StartReceive();
  void AttachRxCallback(UartCallbackFunction &callback);
  [[nodiscard]] const std::vector<u8> &rx_buffer() const;

 private:
  void HalRxCpltCallback(u16 rx_len);

  UartCallbackFunction *rx_callback_{nullptr};
  UART_HandleTypeDef *huart_;
  UartMode tx_mode_;
  UartMode rx_mode_;
  std::vector<u8> rx_buf_[2];
  bool buffer_selector_{false};
};

}  // namespace irobot_ec::hal

#endif
#endif  // EC_LIB_BSP_COMMON_BSP_UART_H

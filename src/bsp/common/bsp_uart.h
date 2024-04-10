
#ifndef EC_LIB_BSP_COMMON_BSP_UART_H
#define EC_LIB_BSP_COMMON_BSP_UART_H

#include "hal_wrapper/hal.h"
#if defined(USE_HAL_UART_REGISTER_CALLBACKS)
#if (USE_HAL_UART_REGISTER_CALLBACKS != 1u)
#error "UART register callback must be enabled!"
#endif
#endif
#if defined(HAL_UART_MODULE_ENABLED)

#include <vector>

namespace irobot_ec::hal {

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
  Uart(UART_HandleTypeDef &huart, size_t rx_size, UartMode tx_mode = UartMode::kNormal,
       UartMode rx_mode = UartMode::kNormal);

  virtual void RxCallback();
  void Write(const uint8_t *data, size_t size);
  void StartReceive();
  [[nodiscard]] const std::vector<uint8_t> &rx_buffer() const;

 private:
  void HalRxCpltCallback();

  UART_HandleTypeDef *huart_;
  UartMode tx_mode_;
  UartMode rx_mode_;
  std::vector<uint8_t> rx_buf_[2];
  bool buffer_selector_{false};
};

}  // namespace irobot_ec::hal

#endif
#endif  // EC_LIB_BSP_COMMON_BSP_UART_H

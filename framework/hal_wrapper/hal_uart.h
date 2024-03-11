
#pragma once

#ifndef EC_LIB_HAL_WRAPPER_HAL_UART_H
#define EC_LIB_HAL_WRAPPER_HAL_UART_H

#include "hal_wrapper/hal_config.h"

#if defined(HAL_UART_MODULE_ENABLED)

#include <unordered_map>

#include "usart.h"

namespace hal::uart {

class UARTEndpointBase {
 public:
  UARTEndpointBase(UART_HandleTypeDef *huart);
  UARTEndpointBase(UART_HandleTypeDef *huart, DMA_HandleTypeDef *hdma_rx);

  UARTEndpointBase() = delete;
  ~UARTEndpointBase() = default;

  void send(const uint8_t *data, uint16_t size);
  void receive(uint8_t *data, uint16_t size);

  virtual void rxCallback(uint8_t *data, uint16_t size) = 0;

 protected:
  UART_HandleTypeDef *huart_;
  DMA_HandleTypeDef *hdma_rx_;

 private:
  static std::unordered_map<UART_HandleTypeDef *, UARTEndpointBase *>
      endpoint_map_;
};

}  // namespace hal::uart

#endif

#endif  // EC_LIB_HAL_WRAPPER_HAL_UART_H

/* EOF */

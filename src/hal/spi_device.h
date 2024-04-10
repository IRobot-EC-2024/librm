/**
 * @file  hal_wrapper/spi_device.h
 * @brief SPI设备抽象
 * @todo  实现不够通用，有改进空间
 */

#ifndef EC_LIB_HAL_WRAPPER_HAL_SPI_H
#define EC_LIB_HAL_WRAPPER_HAL_SPI_H

#include "hal.h"
#if defined(HAL_SPI_MODULE_ENABLED) && defined(HAL_GPIO_MODULE_ENABLED)

#include "modules/typedefs.h"

namespace irobot_ec::hal {
/**
 * @brief SPI设备基类
 */
class SpiDevice {
 public:
  SpiDevice(SPI_HandleTypeDef &hspi, GPIO_TypeDef *cs_gpio_port, u16 cs_pin);
  SpiDevice() = delete;
  ~SpiDevice() = default;

  void ReadByte(u8 reg);
  void ReadBytes(u8 reg, u8 len);
  void WriteByte(u8 reg, u8 data);

  [[nodiscard]] u8 single_byte_buffer() const;
  [[nodiscard]] const u8 *buffer() const;

 protected:
  void ReadWriteByte(u8 tx_data);
  void ReadWriteBytes(u8 reg, u8 len);

  SPI_HandleTypeDef *hspi_;
  GPIO_TypeDef *cs_gpio_port_;
  u16 cs_pin_;
  u8 single_byte_buffer_{};
  u8 buffer_[8]{0};
};
}  // namespace irobot_ec::hal

#endif

#endif  // EC_LIB_HAL_WRAPPER_HAL_SPI_H
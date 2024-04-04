/**
 * @file  hal_wrapper/i2c_device.h
 * @brief i2c设备抽象
 * @todo  实现不够通用，有改进空间
 */

#ifndef EC_LIB_HAL_WRAPPER_I2C_DEVICE_H
#define EC_LIB_HAL_WRAPPER_I2C_DEVICE_H

#include "hal.h"
#if defined(HAL_I2C_MODULE_ENABLED)

namespace irobot_ec::hal {

/**
 * @brief I2C设备
 */
class I2cDevice {
 public:
  I2cDevice(I2C_HandleTypeDef &hi2c, uint8_t addr);
  I2cDevice() = delete;
  ~I2cDevice() = default;

  void Write(uint8_t reg, uint8_t *data, uint16_t size);
  void Read(uint8_t reg, uint16_t size);

  [[nodiscard]] const uint8_t *buffer() const;

 protected:
  I2C_HandleTypeDef *hi2c_;
  uint8_t addr_;
  uint8_t buffer_[8]{0};
};

}  // namespace irobot_ec::hal

#endif

#endif  // EC_LIB_HAL_WRAPPER_I2C_DEVICE_H

/**
 * @file    device/sensor/ist8310.h
 * @brief   IST8310磁力计类
 */

#ifndef EC_LIB_COMPONENTS_SENSOR_IST8310_IST8310_H_
#define EC_LIB_COMPONENTS_SENSOR_IST8310_IST8310_H_

#include "hal/hal.h"
#if defined(HAL_I2C_MODULE_ENABLED)

#include "hal/i2c_device.h"
#include "modules/typedefs.h"

namespace irobot_ec::device::sensor {

enum class IST8310Status : u8 {
  NO_ERROR = 0x00,
  NO_SENSOR = 0x40,
  SENSOR_ERROR = 0x80,
};

/**
 * @brief IST8310磁力计
 */
class IST8310 : public irobot_ec::hal::I2cDevice {
 public:
  IST8310(I2C_HandleTypeDef &hi2c, GPIO_TypeDef *rst_port, u16 rst_pin);
  void Reset();
  void Update();
  [[nodiscard]] IST8310Status status() const;
  [[nodiscard]] f32 mag_x() const;
  [[nodiscard]] f32 mag_y() const;
  [[nodiscard]] f32 mag_z() const;

 private:
  GPIO_TypeDef *rst_port_;
  u16 rst_pin_;
  IST8310Status status_;
  f32 mag_[3]{0};
};

}  // namespace irobot_ec::components::sensor

#endif

#endif  // EC_LIB_COMPONENTS_SENSOR_IST8310_IST8310_H_
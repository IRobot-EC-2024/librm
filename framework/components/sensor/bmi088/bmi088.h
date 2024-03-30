/**
 * @file    components/sensor/bmi088/bmi088.h
 * @brief   BMI088传感器类
 */

#ifndef EC_LIB_COMPONENTS_SENSOR_BMI088_BMI088_H
#define EC_LIB_COMPONENTS_SENSOR_BMI088_BMI088_H

#include "hal_wrapper/hal_config.h"

#ifdef HAL_SPI_MODULE_ENABLED

#include "hal_wrapper/spi_device.h"
#include "modules/typedefs.h"

namespace irobot_ec::components::sensor {

/**
 * @brief BMI088传感器状态
 */
enum class BMI088Status : uint8_t {
  NO_ERROR = 0x00,
  ACC_PWR_CTRL_ERROR = 0x01,
  ACC_PWR_CONF_ERROR = 0x02,
  ACC_CONF_ERROR = 0x03,
  ACC_SELF_TEST_ERROR = 0x04,
  ACC_RANGE_ERROR = 0x05,
  INT1_IO_CTRL_ERROR = 0x06,
  INT_MAP_DATA_ERROR = 0x07,
  GYRO_RANGE_ERROR = 0x08,
  GYRO_BANDWIDTH_ERROR = 0x09,
  GYRO_LPM1_ERROR = 0x0A,
  GYRO_CTRL_ERROR = 0x0B,
  GYRO_INT3_INT4_IO_CONF_ERROR = 0x0C,
  GYRO_INT3_INT4_IO_MAP_ERROR = 0x0D,

  SELF_TEST_ACCEL_ERROR = 0x80,
  SELF_TEST_GYRO_ERROR = 0x40,
  NO_SENSOR = 0xFF,
};

/**
 * @brief BMI088陀螺仪量程
 */
enum class BMI088GyroRange : uint8_t {
  GYRO_2000 = 0u,
  GYRO_1000 = 1u,
  GYRO_500 = 2u,
  GYRO_250 = 3u,
  GYRO_125 = 4u,
};

/**
 * @brief BMI088加速度计量程
 */
enum class BMI088AccelRange : uint8_t {
  ACC_RANGE_3G = 0u,
  ACC_RANGE_6G = 1u,
  ACC_RANGE_12G = 2u,
  ACC_RANGE_24G = 3u,
};

/**
 * @brief BMI088传感器类
 */
class BMI088 {
 public:
  BMI088() = delete;
  ~BMI088() = default;
  BMI088(SPI_HandleTypeDef &hspi, GPIO_TypeDef *cs1_accel_gpio_port, uint16_t cs1_accel_pin,
         GPIO_TypeDef *cs1_gyro_gpio_port, uint16_t cs1_gyro_pin);

  // no copy
  BMI088(const BMI088 &) = delete;
  BMI088 &operator=(const BMI088 &) = delete;

  void Update();
  [[nodiscard]] fp32 GetTemperature() const;
  [[nodiscard]] BMI088Status GetStatus() const;

 private:
  void InitAccelerometer();
  void InitGyroscope();

  hal::SpiDeviceBase accel_device_;
  hal::SpiDeviceBase gyro_device_;

  BMI088Status status_{BMI088Status::NO_SENSOR};
  BMI088GyroRange gyro_range_{BMI088GyroRange::GYRO_2000};
  BMI088AccelRange accel_range_{BMI088AccelRange::ACC_RANGE_3G};

  fp32 gyro_[3]{0};
  fp32 accel_[3]{0};
  fp32 temperature_{};
};
}  // namespace irobot_ec::components::sensor

#endif  // HAL_SPI_MODULE_ENABLED

#endif  // EC_LIB_COMPONENTS_SENSOR_BMI088_BMI088_H
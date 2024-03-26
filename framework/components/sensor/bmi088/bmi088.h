
#ifndef EC_LIB_COMPONENTS_SENSOR_BMI088_BMI088_H_
#define EC_LIB_COMPONENTS_SENSOR_BMI088_BMI088_H_

#include "hal_wrapper/hal_config.h"

#ifdef HAL_SPI_MODULE_ENABLED

#include "modules/typedefs.h"

namespace irobot_ec::components::sensor {

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

class BMI088 {
 public:
  BMI088() = delete;
  ~BMI088() = default;
  BMI088(SPI_HandleTypeDef *hspi, GPIO_TypeDef *cs1_accel_gpio_port, uint16_t cs1_accel_pin, GPIO_TypeDef *cs1_gyro_gpio_port,
         uint16_t cs1_gyro_pin);

  // no copy
  BMI088(const BMI088 &) = delete;
  BMI088 &operator=(const BMI088 &) = delete;

  void Update();
  [[nodiscard]] fp32 GetTemperature() const;

 private:
  void InitAccelerometer();
  void InitGyroscope();

  static void DelayMs(uint16_t ms);
  static void DelayUs(uint16_t us);

  inline void AccelCSLow();
  inline void AccelCSHigh();
  inline void GyroCSLow();
  inline void GyroCSHigh();

  void ReadWriteByte(uint8_t tx_data);
  void ReadWriteBytes(uint8_t reg, uint8_t *buf, uint8_t len);
  [[nodiscard]] BMI088Status GetStatus() const;

  void AccWriteByte(uint8_t reg, uint8_t data);
  void AccReadByte(uint8_t reg, uint8_t *data);
  void AccReadBytes(uint8_t reg, uint8_t *data, uint8_t len);
  void GyroWriteByte(uint8_t reg, uint8_t data);
  void GyroReadByte(uint8_t reg, uint8_t *data);
  void GyroReadBytes(uint8_t reg, uint8_t *data, uint8_t len);

 private:
  GPIO_TypeDef *cs1_accel_gpio_port_;
  uint16_t cs1_accel_pin_;
  GPIO_TypeDef *cs1_gyro_gpio_port_;
  uint16_t cs1_gyro_pin_;
  SPI_HandleTypeDef *hspi_;

  uint8_t read_byte_buffer_;
  BMI088Status status_{BMI088Status::NO_SENSOR};

  fp32 gyro_sen_{0.00106526443603169529841533860381f};
  fp32 accel_sen_{0.0008974358974f};

  uint8_t rx_buffer_[8]{0};

  fp32 gyro_[3]{0};
  fp32 accel_[3]{0};
  fp32 temperature_{};
};

}  // namespace irobot_ec::components::sensor

#endif  // HAL_SPI_MODULE_ENABLED

#endif  // EC_LIB_COMPONENTS_SENSOR_BMI088_BMI088_H_

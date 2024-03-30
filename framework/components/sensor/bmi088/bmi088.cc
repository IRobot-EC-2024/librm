
#include "hal_wrapper/hal_config.h"

#ifdef HAL_SPI_MODULE_ENABLED

#include "bmi088.h"
#include "bmi088_const.hpp"
#include "bsp/bsp.h"

using irobot_ec::bsp::BspFactory;
using namespace irobot_ec::components::sensor;

constexpr static uint8_t write_BMI088_accel_reg_data_error[BMI088_WRITE_ACCEL_REG_NUM][3] = {
    {BMI088_ACC_PWR_CTRL, BMI088_ACC_ENABLE_ACC_ON, static_cast<uint8_t>(BMI088Status::ACC_PWR_CTRL_ERROR)},
    {BMI088_ACC_PWR_CONF, BMI088_ACC_PWR_ACTIVE_MODE, static_cast<uint8_t>(BMI088Status::ACC_PWR_CONF_ERROR)},
    {BMI088_ACC_CONF, BMI088_ACC_NORMAL | BMI088_ACC_800_HZ | BMI088_ACC_CONF_MUST_Set,
     static_cast<uint8_t>(BMI088Status::ACC_CONF_ERROR)},
    {BMI088_ACC_RANGE, BMI088_ACC_RANGE_3G, static_cast<uint8_t>(BMI088Status::ACC_RANGE_ERROR)},
    {BMI088_INT1_IO_CTRL, BMI088_ACC_INT1_IO_ENABLE | BMI088_ACC_INT1_GPIO_PP | BMI088_ACC_INT1_GPIO_LOW,
     static_cast<uint8_t>(BMI088Status::INT1_IO_CTRL_ERROR)},
    {BMI088_INT_MAP_DATA, BMI088_ACC_INT1_DRDY_INTERRUPT, static_cast<uint8_t>(BMI088Status::INT_MAP_DATA_ERROR)}};

constexpr static uint8_t write_BMI088_gyro_reg_data_error[BMI088_WRITE_GYRO_REG_NUM][3] = {
    {BMI088_GYRO_RANGE, BMI088_GYRO_2000, static_cast<uint8_t>(BMI088Status::GYRO_RANGE_ERROR)},
    {BMI088_GYRO_BANDWIDTH, BMI088_GYRO_1000_116_HZ | BMI088_GYRO_BANDWIDTH_MUST_SET,
     static_cast<uint8_t>(BMI088Status::GYRO_BANDWIDTH_ERROR)},
    {BMI088_GYRO_LPM1, BMI088_GYRO_NORMAL_MODE, static_cast<uint8_t>(BMI088Status::GYRO_LPM1_ERROR)},
    {BMI088_GYRO_CTRL, BMI088_DRDY_ON, static_cast<uint8_t>(BMI088Status::GYRO_CTRL_ERROR)},
    {BMI088_GYRO_INT3_INT4_IO_CONF, BMI088_GYRO_INT3_GPIO_PP | BMI088_GYRO_INT3_GPIO_LOW,
     static_cast<uint8_t>(BMI088Status::GYRO_INT3_INT4_IO_CONF_ERROR)},
    {BMI088_GYRO_INT3_INT4_IO_MAP, BMI088_GYRO_DRDY_IO_INT3,
     static_cast<uint8_t>(BMI088Status::GYRO_INT3_INT4_IO_MAP_ERROR)}};

BMI088::BMI088(SPI_HandleTypeDef *hspi, GPIO_TypeDef *cs1_accel_gpio_port, uint16_t cs1_accel_pin,
               GPIO_TypeDef *cs1_gyro_gpio_port, uint16_t cs1_gyro_pin)
    : hspi_(hspi),
      cs1_accel_gpio_port_(cs1_accel_gpio_port),
      cs1_accel_pin_(cs1_accel_pin),
      cs1_gyro_gpio_port_(cs1_gyro_gpio_port),
      cs1_gyro_pin_(cs1_gyro_pin) {
  this->InitGyroscope();
  this->InitAccelerometer();
}

void BMI088::InitAccelerometer() {
  uint8_t res = 0;
  uint8_t write_reg_num = 0;

  // check communication
  this->AccReadByte(BMI088_ACC_CHIP_ID, &res);
  BspFactory::GetDelay().DelayUs(BMI088_COM_WAIT_SENSOR_TIME);
  this->AccReadByte(BMI088_ACC_CHIP_ID, &res);
  BspFactory::GetDelay().DelayUs(BMI088_COM_WAIT_SENSOR_TIME);

  // accel software reset
  this->AccWriteByte(BMI088_ACC_SOFTRESET, BMI088_ACC_SOFTRESET_VALUE);
  BspFactory::GetDelay().DelayMs(BMI088_LONG_DELAY_TIME);

  // check communication is normal after reset
  this->AccReadByte(BMI088_ACC_CHIP_ID, &res);
  BspFactory::GetDelay().DelayUs(BMI088_COM_WAIT_SENSOR_TIME);
  this->AccReadByte(BMI088_ACC_CHIP_ID, &res);
  BspFactory::GetDelay().DelayUs(BMI088_COM_WAIT_SENSOR_TIME);

  // check the "who am I"
  if (res != BMI088_ACC_CHIP_ID_VALUE) {
    this->status_ = BMI088Status::NO_SENSOR;
  }

  // set accel sensor config and check
  for (write_reg_num = 0; write_reg_num < BMI088_WRITE_ACCEL_REG_NUM; write_reg_num++) {
    this->AccWriteByte(write_BMI088_accel_reg_data_error[write_reg_num][0],
                       write_BMI088_accel_reg_data_error[write_reg_num][1]);
    BspFactory::GetDelay().DelayUs(BMI088_COM_WAIT_SENSOR_TIME);

    this->AccReadByte(write_BMI088_accel_reg_data_error[write_reg_num][0], &res);
    BspFactory::GetDelay().DelayUs(BMI088_COM_WAIT_SENSOR_TIME);

    if (res != write_BMI088_accel_reg_data_error[write_reg_num][1]) {
      this->status_ = (BMI088Status)write_BMI088_accel_reg_data_error[write_reg_num][2];
    }
  }
  this->status_ = BMI088Status::NO_ERROR;
}

void BMI088::InitGyroscope() {
  uint8_t write_reg_num = 0;
  uint8_t res = 0;

  // check communication
  this->GyroReadByte(BMI088_GYRO_CHIP_ID, &res);
  BspFactory::GetDelay().DelayUs(BMI088_COM_WAIT_SENSOR_TIME);
  this->GyroReadByte(BMI088_GYRO_CHIP_ID, &res);
  BspFactory::GetDelay().DelayUs(BMI088_COM_WAIT_SENSOR_TIME);

  // reset the gyro sensor
  this->GyroWriteByte(BMI088_GYRO_SOFTRESET, BMI088_GYRO_SOFTRESET_VALUE);
  BspFactory::GetDelay().DelayMs(BMI088_LONG_DELAY_TIME);

  // check communication is normal after reset
  this->GyroReadByte(BMI088_GYRO_CHIP_ID, &res);
  BspFactory::GetDelay().DelayUs(BMI088_COM_WAIT_SENSOR_TIME);
  this->GyroReadByte(BMI088_GYRO_CHIP_ID, &res);
  BspFactory::GetDelay().DelayUs(BMI088_COM_WAIT_SENSOR_TIME);

  // check the "who am I"
  if (res != BMI088_GYRO_CHIP_ID_VALUE) {
    this->status_ = BMI088Status::NO_SENSOR;
  }

  // set gyro sensor config and check
  for (write_reg_num = 0; write_reg_num < BMI088_WRITE_GYRO_REG_NUM; write_reg_num++) {
    this->GyroWriteByte(write_BMI088_gyro_reg_data_error[write_reg_num][0],
                        write_BMI088_gyro_reg_data_error[write_reg_num][1]);
    BspFactory::GetDelay().DelayUs(BMI088_COM_WAIT_SENSOR_TIME);

    this->GyroReadByte(write_BMI088_gyro_reg_data_error[write_reg_num][0], &res);
    BspFactory::GetDelay().DelayUs(BMI088_COM_WAIT_SENSOR_TIME);

    if (res != write_BMI088_gyro_reg_data_error[write_reg_num][1]) {
      this->status_ = (BMI088Status)write_BMI088_gyro_reg_data_error[write_reg_num][2];
    }
  }

  this->status_ = BMI088Status::NO_ERROR;
}

void BMI088::Update() {
  uint8_t buf[8];
  int16_t bmi088_raw_temp;

  this->AccReadBytes(BMI088_ACCEL_XOUT_L, buf, 6);

  bmi088_raw_temp = (int16_t)((buf[1]) << 8) | buf[0];
  this->accel_[0] = bmi088_raw_temp * this->accel_sen_;
  bmi088_raw_temp = (int16_t)((buf[3]) << 8) | buf[2];
  this->accel_[1] = bmi088_raw_temp * this->accel_sen_;
  bmi088_raw_temp = (int16_t)((buf[5]) << 8) | buf[4];
  this->accel_[2] = bmi088_raw_temp * this->accel_sen_;

  this->GyroReadBytes(BMI088_GYRO_CHIP_ID, buf, 8);
  if (buf[0] == BMI088_GYRO_CHIP_ID_VALUE) {
    bmi088_raw_temp = (int16_t)((buf[3]) << 8) | buf[2];
    this->gyro_[0] = bmi088_raw_temp * this->gyro_sen_;
    bmi088_raw_temp = (int16_t)((buf[5]) << 8) | buf[4];
    this->gyro_[1] = bmi088_raw_temp * this->gyro_sen_;
    bmi088_raw_temp = (int16_t)((buf[7]) << 8) | buf[6];
    this->gyro_[2] = bmi088_raw_temp * this->gyro_sen_;
  }
  this->AccReadBytes(BMI088_TEMP_M, buf, 2);

  bmi088_raw_temp = (int16_t)((buf[0] << 3) | (buf[1] >> 5));

  if (bmi088_raw_temp > 1023) {
    bmi088_raw_temp -= 2048;
  }

  this->temperature_ = bmi088_raw_temp * BMI088_TEMP_FACTOR + BMI088_TEMP_OFFSET;
}

inline void BMI088::AccelCSHigh() { HAL_GPIO_WritePin(this->cs1_accel_gpio_port_, this->cs1_accel_pin_, GPIO_PIN_SET); }

inline void BMI088::AccelCSLow() {
  HAL_GPIO_WritePin(this->cs1_accel_gpio_port_, this->cs1_accel_pin_, GPIO_PIN_RESET);
}

inline void BMI088::GyroCSHigh() { HAL_GPIO_WritePin(this->cs1_gyro_gpio_port_, this->cs1_gyro_pin_, GPIO_PIN_SET); }

inline void BMI088::GyroCSLow() { HAL_GPIO_WritePin(this->cs1_gyro_gpio_port_, this->cs1_gyro_pin_, GPIO_PIN_RESET); }

void BMI088::ReadWriteByte(uint8_t tx_data) {
  HAL_SPI_TransmitReceive(this->hspi_, &tx_data, &this->read_byte_buffer_, 1, 1000);
}

void BMI088::ReadWriteBytes(uint8_t reg, uint8_t *buf, uint8_t len) {
  this->ReadWriteByte(reg | 0x80);

  while (len != 0) {
    this->ReadWriteByte(0x55);
    *buf = this->read_byte_buffer_;
    buf++;
    len--;
  }
}

BMI088Status BMI088::GetStatus() const { return this->status_; }
fp32 BMI088::GetTemperature() const { return this->temperature_; }

void BMI088::AccWriteByte(uint8_t reg, uint8_t data) {
  this->AccelCSLow();
  this->ReadWriteByte(reg);
  this->ReadWriteByte(data);
  this->AccelCSHigh();
}

void BMI088::AccReadByte(uint8_t reg, uint8_t *data) {
  this->AccelCSLow();
  this->ReadWriteByte(reg | 0x80);
  this->ReadWriteByte(0x55);
  this->AccelCSHigh();
}

void BMI088::AccReadBytes(uint8_t reg, uint8_t *data, uint8_t len) {
  this->AccelCSLow();
  this->ReadWriteByte(reg | 0x80);
  this->ReadWriteBytes(reg, data, len);
  this->AccelCSHigh();
}

void BMI088::GyroWriteByte(uint8_t reg, uint8_t data) {
  this->GyroCSLow();
  this->ReadWriteByte(reg);
  this->ReadWriteByte(data);
  this->GyroCSHigh();
}

void BMI088::GyroReadByte(uint8_t reg, uint8_t *data) {
  this->GyroCSLow();
  this->ReadWriteByte(reg | 0x80);
  this->ReadWriteByte(0x55);
  *data = this->read_byte_buffer_;
  this->GyroCSHigh();
}

void BMI088::GyroReadBytes(uint8_t reg, uint8_t *data, uint8_t len) {
  this->GyroCSLow();
  this->ReadWriteBytes(reg, data, len);
  this->GyroCSHigh();
}

#endif
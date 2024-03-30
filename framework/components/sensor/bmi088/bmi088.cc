/**
 * @file    components/sensor/bmi088/bmi088.cc
 * @brief   BMI088传感器类
 */

#include "hal_wrapper/hal.h"

#ifdef HAL_SPI_MODULE_ENABLED

#include <vector>

#include "bmi088.h"
#include "bmi088_const.hpp"
#include "bsp/bsp.h"

using irobot_ec::bsp::BspFactory;
using namespace irobot_ec::components::sensor;

/**
 * @brief BMI088加速度计初始化序列
 * @note  顺序：寄存器地址，写入值，错误码
 */
const static std::vector<std::vector<uint8_t>> BMI088_ACCEL_INIT_SEQUENCE = {
    {BMI088_ACC_PWR_CTRL, BMI088_ACC_ENABLE_ACC_ON, static_cast<uint8_t>(BMI088Status::ACC_PWR_CTRL_ERROR)},
    {BMI088_ACC_PWR_CONF, BMI088_ACC_PWR_ACTIVE_MODE, static_cast<uint8_t>(BMI088Status::ACC_PWR_CONF_ERROR)},
    {BMI088_ACC_CONF, BMI088_ACC_NORMAL | BMI088_ACC_800_HZ | BMI088_ACC_CONF_MUST_Set,
     static_cast<uint8_t>(BMI088Status::ACC_CONF_ERROR)},
    {BMI088_ACC_RANGE, BMI088_ACC_RANGE_3G, static_cast<uint8_t>(BMI088Status::ACC_RANGE_ERROR)},
    {BMI088_INT1_IO_CTRL, BMI088_ACC_INT1_IO_ENABLE | BMI088_ACC_INT1_GPIO_PP | BMI088_ACC_INT1_GPIO_LOW,
     static_cast<uint8_t>(BMI088Status::INT1_IO_CTRL_ERROR)},
    {BMI088_INT_MAP_DATA, BMI088_ACC_INT1_DRDY_INTERRUPT, static_cast<uint8_t>(BMI088Status::INT_MAP_DATA_ERROR)}};

/**
 * @brief BMI088陀螺仪初始化序列
 * @note  顺序：寄存器地址，写入值，错误码
 */
const static std::vector<std::vector<uint8_t>> BMI088_GYRO_INIT_SEQUENCE = {
    {BMI088_GYRO_RANGE, BMI088_GYRO_2000, static_cast<uint8_t>(BMI088Status::GYRO_RANGE_ERROR)},
    {BMI088_GYRO_BANDWIDTH, BMI088_GYRO_1000_116_HZ | BMI088_GYRO_BANDWIDTH_MUST_SET,
     static_cast<uint8_t>(BMI088Status::GYRO_BANDWIDTH_ERROR)},
    {BMI088_GYRO_LPM1, BMI088_GYRO_NORMAL_MODE, static_cast<uint8_t>(BMI088Status::GYRO_LPM1_ERROR)},
    {BMI088_GYRO_CTRL, BMI088_DRDY_ON, static_cast<uint8_t>(BMI088Status::GYRO_CTRL_ERROR)},
    {BMI088_GYRO_INT3_INT4_IO_CONF, BMI088_GYRO_INT3_GPIO_PP | BMI088_GYRO_INT3_GPIO_LOW,
     static_cast<uint8_t>(BMI088Status::GYRO_INT3_INT4_IO_CONF_ERROR)},
    {BMI088_GYRO_INT3_INT4_IO_MAP, BMI088_GYRO_DRDY_IO_INT3,
     static_cast<uint8_t>(BMI088Status::GYRO_INT3_INT4_IO_MAP_ERROR)}};

/**
 * @param hspi                  SPI外设句柄
 * @param cs1_accel_gpio_port   加速度计片选引脚所在GPIO端口
 * @param cs1_accel_pin         加速度计片选引脚编号
 * @param cs1_gyro_gpio_port    陀螺仪片选引脚所在GPIO端口
 * @param cs1_gyro_pin          陀螺仪片选引脚编号
 */
BMI088::BMI088(SPI_HandleTypeDef &hspi, GPIO_TypeDef *cs1_accel_gpio_port, uint16_t cs1_accel_pin,
               GPIO_TypeDef *cs1_gyro_gpio_port, uint16_t cs1_gyro_pin)
    : accel_device_(hspi, cs1_accel_gpio_port, cs1_accel_pin), gyro_device_(hspi, cs1_gyro_gpio_port, cs1_gyro_pin) {
  this->InitGyroscope();
  this->InitAccelerometer();
}

/**
 * @brief 初始化加速度计
 */
void BMI088::InitAccelerometer() {
  // 检查通信是否正常
  this->accel_device_.ReadByte(BMI088_ACC_CHIP_ID);
  BspFactory::GetDelay().DelayUs(BMI088_COM_WAIT_SENSOR_TIME);
  this->accel_device_.ReadByte(BMI088_ACC_CHIP_ID);
  BspFactory::GetDelay().DelayUs(BMI088_COM_WAIT_SENSOR_TIME);

  // soft reset一次
  this->accel_device_.WriteByte(BMI088_ACC_SOFTRESET, BMI088_ACC_SOFTRESET_VALUE);
  BspFactory::GetDelay().DelayMs(BMI088_LONG_DELAY_TIME);

  // 再次检查通信是否正常
  this->accel_device_.ReadByte(BMI088_ACC_CHIP_ID);
  BspFactory::GetDelay().DelayUs(BMI088_COM_WAIT_SENSOR_TIME);
  this->accel_device_.ReadByte(BMI088_ACC_CHIP_ID);
  BspFactory::GetDelay().DelayUs(BMI088_COM_WAIT_SENSOR_TIME);

  // 检查"who am I"寄存器值是否正确
  if (accel_device_.single_byte_buffer() != BMI088_ACC_CHIP_ID_VALUE) {
    this->status_ = BMI088Status::NO_SENSOR;
  }

  // 发送初始化序列，有错误则设置status为对应错误码
  for (const auto &operation : BMI088_ACCEL_INIT_SEQUENCE) {
    this->accel_device_.WriteByte(operation[0], operation[1]);
    BspFactory::GetDelay().DelayUs(BMI088_COM_WAIT_SENSOR_TIME);
    this->accel_device_.ReadByte(operation[0]);
    BspFactory::GetDelay().DelayUs(BMI088_COM_WAIT_SENSOR_TIME);

    if (accel_device_.single_byte_buffer() != operation[1]) {
      this->status_ = (BMI088Status)operation[2];
    }
  }
  this->status_ = BMI088Status::NO_ERROR;
}

/**
 * @brief 初始化陀螺仪
 */
void BMI088::InitGyroscope() {
  // 检查通信是否正常
  this->gyro_device_.ReadByte(BMI088_GYRO_CHIP_ID);
  BspFactory::GetDelay().DelayUs(BMI088_COM_WAIT_SENSOR_TIME);
  this->gyro_device_.ReadByte(BMI088_GYRO_CHIP_ID);
  BspFactory::GetDelay().DelayUs(BMI088_COM_WAIT_SENSOR_TIME);

  // soft reset一次
  this->gyro_device_.WriteByte(BMI088_GYRO_SOFTRESET, BMI088_GYRO_SOFTRESET_VALUE);
  BspFactory::GetDelay().DelayMs(BMI088_LONG_DELAY_TIME);

  // 再次检查通信是否正常
  this->gyro_device_.ReadByte(BMI088_GYRO_CHIP_ID);
  BspFactory::GetDelay().DelayUs(BMI088_COM_WAIT_SENSOR_TIME);
  this->gyro_device_.ReadByte(BMI088_GYRO_CHIP_ID);
  BspFactory::GetDelay().DelayUs(BMI088_COM_WAIT_SENSOR_TIME);

  // 检查"who am I"寄存器值是否正确
  if (gyro_device_.single_byte_buffer() != BMI088_GYRO_CHIP_ID_VALUE) {
    this->status_ = BMI088Status::NO_SENSOR;
  }

  // 发送初始化序列，有错误则设置status为对应错误码
  for (const auto &operation : BMI088_GYRO_INIT_SEQUENCE) {
    this->gyro_device_.WriteByte(operation[0], operation[1]);
    BspFactory::GetDelay().DelayUs(BMI088_COM_WAIT_SENSOR_TIME);
    this->gyro_device_.ReadByte(operation[0]);
    BspFactory::GetDelay().DelayUs(BMI088_COM_WAIT_SENSOR_TIME);

    if (gyro_device_.single_byte_buffer() != operation[1]) {
      this->status_ = (BMI088Status)operation[2];
    }
  }

  this->status_ = BMI088Status::NO_ERROR;
}

/**
 * @brief 更新传感器数据
 */
void BMI088::Update() {
  const uint8_t *buf = this->accel_device_.buffer();

  this->accel_device_.ReadBytes(BMI088_ACCEL_XOUT_L, 6);
  this->accel_[0] = ((int16_t)((buf[1]) << 8) | buf[0]) * BMI088_ACCEL_SENSITIVITY[(uint8_t)this->accel_range_];
  this->accel_[1] = ((int16_t)((buf[3]) << 8) | buf[2]) * BMI088_ACCEL_SENSITIVITY[(uint8_t)this->accel_range_];
  this->accel_[2] = ((int16_t)((buf[5]) << 8) | buf[4]) * BMI088_ACCEL_SENSITIVITY[(uint8_t)this->accel_range_];

  this->gyro_device_.ReadBytes(BMI088_GYRO_CHIP_ID, 8);
  if (buf[0] == BMI088_GYRO_CHIP_ID_VALUE) {
    this->gyro_[0] = ((int16_t)((buf[3]) << 8) | buf[2]) * BMI088_GYRO_SENSITIVITY[(uint8_t)this->gyro_range_];
    this->gyro_[1] = ((int16_t)((buf[5]) << 8) | buf[4]) * BMI088_GYRO_SENSITIVITY[(uint8_t)this->gyro_range_];
    this->gyro_[2] = ((int16_t)((buf[7]) << 8) | buf[6]) * BMI088_GYRO_SENSITIVITY[(uint8_t)this->gyro_range_];
  }
  this->accel_device_.ReadBytes(BMI088_TEMP_M, 2);

  auto bmi088_raw_temp = (int16_t)((buf[0] << 3) | (buf[1] >> 5));
  if (bmi088_raw_temp > 1023) {
    bmi088_raw_temp -= 2048;
  }
  this->temperature_ = bmi088_raw_temp * BMI088_TEMP_FACTOR + BMI088_TEMP_OFFSET;
}

/**
 * @brief   获取传感器状态
 * @return  传感器状态
 */
BMI088Status BMI088::GetStatus() const { return this->status_; }

/**
 * @brief   获取传感器温度
 * @return  传感器温度
 */
fp32 BMI088::GetTemperature() const { return this->temperature_; }

#endif
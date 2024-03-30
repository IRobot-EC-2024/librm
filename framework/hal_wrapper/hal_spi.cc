/**
 * @file  hal_wrapper/hal_spi.cc
 * @brief SPI设备包装类
 */

#include "hal_spi.h"

#include "hal_config.h"
#if defined(HAL_SPI_MODULE_ENABLED) && defined(HAL_GPIO_MODULE_ENABLED)

namespace irobot_ec::hal {
/**
 * @param hspi          SPI外设句柄
 * @param cs_gpio_port  片选引脚所在GPIO端口
 * @param cs_pin        片选引脚编号
 */
SpiDeviceBase::SpiDeviceBase(SPI_HandleTypeDef &hspi, GPIO_TypeDef *cs_gpio_port, uint16_t cs_pin)
    : hspi_(&hspi), cs_gpio_port_(cs_gpio_port), cs_pin_(cs_pin) {
  HAL_GPIO_WritePin(cs_gpio_port_, cs_pin_, GPIO_PIN_SET);
}

/**
 * @brief 读写一个字节
 * @param tx_data 待发送的数据
 */
void SpiDeviceBase::ReadWriteByte(uint8_t tx_data) {
  HAL_SPI_TransmitReceive(this->hspi_, &tx_data, &this->single_byte_buffer_, 1, 1000);
}

/**
 * @brief 读写多个字节
 * @param reg 寄存器地址
 * @param len 待读取的字节数
 */
void SpiDeviceBase::ReadWriteBytes(const uint8_t reg, const uint8_t len) {
  this->ReadWriteByte(reg | 0x80);

  for (int i = 0; i < len; i++) {
    this->ReadWriteByte(0x55);
    this->buffer_[i] = this->single_byte_buffer_;
  }
}

/**
 * @brief 写一个字节
 * @param reg   寄存器地址
 * @param data  待写入的数据
 */
void SpiDeviceBase::WriteByte(uint8_t reg, uint8_t data) {
  HAL_GPIO_WritePin(this->cs_gpio_port_, this->cs_pin_, GPIO_PIN_RESET);
  this->ReadWriteByte(reg);
  this->ReadWriteByte(data);
  HAL_GPIO_WritePin(this->cs_gpio_port_, this->cs_pin_, GPIO_PIN_SET);
}

/**
 * @brief   获取单字节缓冲区的值
 * @return  单字节缓冲区的值
 */
uint8_t SpiDeviceBase::single_byte_buffer() const { return this->single_byte_buffer_; }

/**
 * @brief   获取缓冲区指针
 * @return  缓冲区指针
 */
const uint8_t *SpiDeviceBase::buffer() const { return this->buffer_; }

/**
 * @brief 读一个字节
 * @param reg 寄存器地址
 */
void SpiDeviceBase::ReadByte(uint8_t reg) {
  HAL_GPIO_WritePin(this->cs_gpio_port_, this->cs_pin_, GPIO_PIN_RESET);
  this->ReadWriteByte(reg | 0x80);
  this->ReadWriteByte(0x55);
  HAL_GPIO_WritePin(this->cs_gpio_port_, this->cs_pin_, GPIO_PIN_SET);
}

/**
 * @brief 读多个字节
 * @param reg 寄存器地址
 * @param len 待读取的字节数
 */
void SpiDeviceBase::ReadBytes(uint8_t reg, uint8_t len) {
  HAL_GPIO_WritePin(this->cs_gpio_port_, this->cs_pin_, GPIO_PIN_RESET);
  this->ReadWriteByte(reg | 0x80);
  this->ReadWriteBytes(reg, len);
  HAL_GPIO_WritePin(this->cs_gpio_port_, this->cs_pin_, GPIO_PIN_SET);
}
}  // namespace irobot_ec::hal

#endif
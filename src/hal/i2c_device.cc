/**
 * @file  hal/i2c_device.cc
 * @brief i2c设备抽象
 * @todo  实现不够通用，有改进空间
 */

#include "hal.h"
#if defined(HAL_I2C_MODULE_ENABLED)

#include "i2c_device.h"

#include "modules/typedefs.h"

namespace irobot_ec::hal {

/**
 * @param hi2c I2C外设句柄
 * @param addr I2C设备地址
 */
I2cDevice::I2cDevice(I2C_HandleTypeDef &hi2c, u8 addr) : hi2c_(&hi2c), addr_(addr) {}

/**
 * @brief 写数据
 * @param reg   寄存器地址
 * @param data  待写入的数据
 * @param size  数据长度
 */
void I2cDevice::Write(u8 reg, u8 *data, uint16_t size) {
  HAL_I2C_Mem_Write(this->hi2c_, this->addr_ << 1, reg, I2C_MEMADD_SIZE_8BIT, data, size, 10);
}

/**
 * @brief 读数据
 * @param reg   寄存器地址
 * @param size  数据长度
 */
void I2cDevice::Read(u8 reg, uint16_t size) {
  HAL_I2C_Mem_Read(this->hi2c_, this->addr_ << 1, reg, I2C_MEMADD_SIZE_8BIT, this->buffer_, size, 1000);
}

/**
 * @brief 获取缓冲区指针
 * @return 缓冲区指针
 */
const u8 *I2cDevice::buffer() const { return this->buffer_; }

}  // namespace irobot_ec::hal

#endif
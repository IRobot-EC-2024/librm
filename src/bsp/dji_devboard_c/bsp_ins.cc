/**
 * @file    bsp/dji_devboard_c/bsp_ins.h
 * @brief   大疆C板惯导
 * @todo    温控未实现
 */

#include "hal_wrapper/hal.h"

#if defined(HAL_I2C_MODULE_ENABLED) && defined(HAL_SPI_MODULE_ENABLED) && defined(HAL_TIM_MODULE_ENABLED) && \
    defined(STM32F407xx)

#include "bsp_ins.h"

namespace irobot_ec::bsp::dji_devboard_c {

/**
 * @param sample_rate 惯导的更新频率，默认值1000hz
 */
Ins::Ins(fp32 sample_rate)
    : bmi088_(hspi1, GPIOA, GPIO_PIN_4, GPIOB, GPIO_PIN_0),
      ist8310_(hi2c3, GPIOG, GPIO_PIN_6),
      mahony_(sample_rate),
      heater_pwm_(&htim10) {
  HAL_TIM_PWM_Start(heater_pwm_, TIM_CHANNEL_1);
}

fp32 Ins::acc_x() const { return this->acc_[0]; }
fp32 Ins::acc_y() const { return this->acc_[1]; }
fp32 Ins::acc_z() const { return this->acc_[2]; }
fp32 Ins::gyro_x() const { return this->gyro_[0]; }
fp32 Ins::gyro_y() const { return this->gyro_[1]; }
fp32 Ins::gyro_z() const { return this->gyro_[2]; }
fp32 Ins::mag_x() const { return this->mag_[0]; }
fp32 Ins::mag_y() const { return this->mag_[1]; }
fp32 Ins::mag_z() const { return this->mag_[2]; }
fp32 Ins::yaw() const { return this->ypr_[0]; }
fp32 Ins::pitch() const { return this->ypr_[1]; }
fp32 Ins::roll() const { return this->ypr_[2]; }
fp32 Ins::quat_w() const { return this->quaternion_[0]; }
fp32 Ins::quat_x() const { return this->quaternion_[1]; }
fp32 Ins::quat_y() const { return this->quaternion_[2]; }
fp32 Ins::quat_z() const { return this->quaternion_[3]; }
fp32 Ins::temperature() const { return this->temperature_; }

/**
 * @brief     更新惯导数据
 */
void Ins::Update() {
  this->bmi088_.Update();
  this->ist8310_.Update();
  this->acc_[0] = this->bmi088_.accel_x();
  this->acc_[1] = this->bmi088_.accel_y();
  this->acc_[2] = this->bmi088_.accel_z();
  this->gyro_[0] = this->bmi088_.gyro_x();
  this->gyro_[1] = this->bmi088_.gyro_y();
  this->gyro_[2] = this->bmi088_.gyro_z();
  this->mag_[0] = this->ist8310_.mag_x();
  this->mag_[1] = this->ist8310_.mag_y();
  this->mag_[2] = this->ist8310_.mag_z();
  this->temperature_ = this->bmi088_.temperature();
  if (this->bypass_mag_) {
    this->mahony_.UpdateImu(this->gyro_[0], this->gyro_[1], this->gyro_[2], this->acc_[0], this->acc_[1],
                            this->acc_[2]);
  } else {
    this->mahony_.Update(this->gyro_[0], this->gyro_[1], this->gyro_[2], this->acc_[0], this->acc_[1], this->acc_[2],
                         this->mag_[0], this->mag_[1], this->mag_[2]);
  }
  this->ypr_[0] = this->mahony_.euler_yaw();
  this->ypr_[1] = this->mahony_.euler_pitch();
  this->ypr_[2] = this->mahony_.euler_roll();
  this->quaternion_[0] = this->mahony_.quat_w();
  this->quaternion_[1] = this->mahony_.quat_x();
  this->quaternion_[2] = this->mahony_.quat_y();
  this->quaternion_[3] = this->mahony_.quat_z();
}

/**
 * @brief         设置是否禁用磁力计
 * @param bypass  是否禁用磁力计
 */
void Ins::BypassMagnetometer(bool bypass) { this->bypass_mag_ = bypass; }

}  // namespace irobot_ec::bsp::dji_devboard_c

#endif

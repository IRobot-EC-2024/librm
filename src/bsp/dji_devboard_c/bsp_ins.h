/**
 * @file    bsp/dji_devboard_c/bsp_ins.h
 * @brief   大疆C板惯导
 * @todo    温控未实现
 */

#ifndef EC_LIB_DJI_C_BSP_INS_H
#define EC_LIB_DJI_C_BSP_INS_H

#include "hal/hal.h"
#if defined(HAL_I2C_MODULE_ENABLED) && defined(HAL_SPI_MODULE_ENABLED) && defined(HAL_TIM_MODULE_ENABLED) && \
    defined(STM32F407xx)

#include "bsp/interface/bsp_ins_interface.h"
#include "device/sensor/bmi088/bmi088.h"
#include "device/sensor/ist8310/ist8310.h"
#include "modules/algorithm/mahony_ahrs.h"
#include "modules/typedefs.h"

namespace irobot_ec::bsp::dji_devboard_c {

/**
 * @brief 大疆C板惯导功能实现
 * @note  单例类，不能实例化，使用时通过bsp工厂获取实例
 */
class Ins : public InsInterface {
 public:
  explicit Ins(f32 sample_rate = 1000.0f);
  ~Ins() override = default;

  [[nodiscard]] f32 acc_x() const override;
  [[nodiscard]] f32 acc_y() const override;
  [[nodiscard]] f32 acc_z() const override;
  [[nodiscard]] f32 gyro_x() const override;
  [[nodiscard]] f32 gyro_y() const override;
  [[nodiscard]] f32 gyro_z() const override;
  [[nodiscard]] f32 mag_x() const override;
  [[nodiscard]] f32 mag_y() const override;
  [[nodiscard]] f32 mag_z() const override;
  [[nodiscard]] f32 yaw() const override;
  [[nodiscard]] f32 pitch() const override;
  [[nodiscard]] f32 roll() const override;
  [[nodiscard]] f32 quat_w() const override;
  [[nodiscard]] f32 quat_x() const override;
  [[nodiscard]] f32 quat_y() const override;
  [[nodiscard]] f32 quat_z() const override;
  [[nodiscard]] f32 temperature() const;

  void Update() override;
  void BypassMagnetometer(bool bypass);

 private:
  device::sensor::BMI088 bmi088_;
  device::sensor::IST8310 ist8310_;
  modules::algorithm::MahonyAhrs mahony_;
  TIM_HandleTypeDef *heater_pwm_;

  f32 acc_[3]{0};
  f32 gyro_[3]{0};
  f32 mag_[3]{0};
  f32 ypr_[3]{0};
  f32 quaternion_[4]{0};
  f32 temperature_{};

  bool bypass_mag_{true};
};

}  // namespace irobot_ec::bsp::dji_devboard_c

#endif

#endif  // EC_LIB_DJI_C_BSP_INS_H

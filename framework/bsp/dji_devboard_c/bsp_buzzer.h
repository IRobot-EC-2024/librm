
#ifndef EC_LIB_DJI_C_BSP_BUZZER_H
#define EC_LIB_DJI_C_BSP_BUZZER_H

#include "hal_wrapper/hal.h"

#if defined(HAL_TIM_MODULE_ENABLED) && defined(STM32F407xx)

#include "tim.h"

namespace irobot_ec::bsp::dji_devboard_c::buzzer {

class Buzzer {
 public:
  Buzzer(TIM_HandleTypeDef htim);
  Buzzer() = delete;
  ~Buzzer() = default;

  void on();
  void off();

 private:
  TIM_HandleTypeDef *htim;
};

}  // namespace irobot_ec::bsp::dji_devboard_c::buzzer

#endif

#endif  // EC_LIB_DJI_C_BSP_BUZZER_H

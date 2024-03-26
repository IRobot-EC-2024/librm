
#ifndef EC_LIB_DJI_C_BSP_LED_H
#define EC_LIB_DJI_C_BSP_LED_H

#include "hal_wrapper/hal_config.h"

#if defined(HAL_TIM_MODULE_ENABLED) && defined(HAL_GPIO_MODULE_ENABLED) && defined(STM32F407xx)

#include "tim.h"

namespace irobot_ec::bsp::dji_devboard_c::led {

class LED {
 public:
  LED(TIM_HandleTypeDef htim);
  LED() = delete;
  ~LED() = default;

  void on();
  void off();

 private:
  TIM_HandleTypeDef *htim;
};

}  // namespace irobot_ec::bsp::dji_devboard_c::led

#endif

#endif  // EC_LIB_DJI_C_BSP_LED_H

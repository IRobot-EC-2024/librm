
#ifndef EC_LIB_DJI_C_BSP_DELAY_H
#define EC_LIB_DJI_C_BSP_DELAY_H

#include "hal_wrapper/hal_config.h"

#if defined(HAL_TIM_MODULE_ENABLED) && defined(STM32F407xx)

#include "modules/typedefs.h"

namespace bsp::dji_devboard_c {

void delayUs(uint16_t us);

void delayMs(uint16_t ms);

}  // namespace bsp::dji_devboard_c

#endif

#endif  // EC_LIB_DJI_C_BSP_DELAY_H

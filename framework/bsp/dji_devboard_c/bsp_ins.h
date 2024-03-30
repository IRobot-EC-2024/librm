
#ifndef EC_LIB_DJI_C_BSP_INS_H
#define EC_LIB_DJI_C_BSP_INS_H

#include "hal_wrapper/hal.h"

#if defined(HAL_I2C_MODULE_ENABLED) && defined(HAL_SPI_MODULE_ENABLED) && defined(STM32F407xx)

namespace irobot_ec::bsp::dji_devboard_c {

class Ins {
 public:
};

}  // namespace irobot_ec::bsp::dji_devboard_c

#endif

#endif  // EC_LIB_DJI_C_BSP_INS_H

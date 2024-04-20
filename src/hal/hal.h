/**
 * @file  hal/hal.h
 * @brief 根据编译宏定义引入不同的芯片型号的HAL库
 */

#ifndef EC_LIB_HAL_WRAPPER_HAL_CONFIG_H
#define EC_LIB_HAL_WRAPPER_HAL_CONFIG_H

#if defined(STM32F407xx)
#include "stm32f407xx.h"
#include "stm32f4xx_hal.h"
#elif defined(STM32H723xx)
#include "stm32h723xx.h"
#include "stm32h7xx_hal.h"
#else
#error "No supported chip defined"
#endif

#endif  // EC_LIB_HAL_CONFIG_H

/* EOF */

/***
 * @file hal_wrapper/hal_config.h
 * @brief 通过引入不同的芯片型号的HAL库配置文件，实现本库的跨平台支持
 */

#pragma once

#ifndef EC_LIB_HAL_WRAPPER_HAL_CONFIG_H
#define EC_LIB_HAL_WRAPPER_HAL_CONFIG_H

#if defined(STM32F407xx)
#include "stm32f4xx_hal.h"
#elif defined(STM32H723xx)
#include "stm32h7xx_hal.h"
#else
#error "No supported chip defined"
#endif

#endif // EC_LIB_HAL_CONFIG_H

/* EOF */

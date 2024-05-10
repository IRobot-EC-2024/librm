/**
 * @file  modules/freertos.h
 * @brief 检测FreeRTOS是否可用
 */

#ifndef EC_LIB_MODULES_FREERTOS_H
#define EC_LIB_MODULES_FREERTOS_H

#if defined(__GNUC__) || defined(__clang__)
#if __has_include("FreeRTOS.h")
#define EC_LIB_USE_FREERTOS
#else
#warning "FreeRTOS is not available."
#endif
#else
#error "Unsupported compiler, please use GCC or Clang."
#endif

#endif  // EC_LIB_MODULES_FREERTOS_H

/**
 * @file      modules/dsp.h
 * @brief     简化导入DSP库的语句，引入此文件即可使用DSP库（如果支持并且启用了的话）
 * @attention 检查包含路径和链接器设置，确保DSP库确实已经正确启用！！
 */

#ifndef EC_LIB_HAL_WRAPPER_DSP_H
#define EC_LIB_HAL_WRAPPER_DSP_H

#if defined(__GNUC__) || defined(__clang__)  // GCC or Clang
#if __has_include("arm_math.h")              // C++17 or above
#include "hal.h"                             // 这一句是为了导入__FPU_PRESENT宏
#include "arm_math.h"
#else
#warning "CMSIS-DSP library is not available."
#endif
#else
#error "Unsupported compiler, please use GCC or Clang."
#endif

#endif  // EC_LIB_HAL_WRAPPER_DSP_H

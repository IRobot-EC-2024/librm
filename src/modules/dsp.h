/*
  Copyright (c) 2024 XDU-IRobot

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
*/

/**
 * @file      modules/dsp.h
 * @brief     简化导入DSP库的语句，引入此文件即可使用DSP库（如果支持并且启用了的话）
 * @attention 检查包含路径和链接器设置，确保DSP库确实已经正确启用！！
 */

#ifndef EC_LIB_HAL_WRAPPER_DSP_H
#define EC_LIB_HAL_WRAPPER_DSP_H

#if defined(__GNUC__) || defined(__clang__)  // GCC or Clang
#if __has_include("arm_math.h")              // C++17 or above
#include "hal/hal.h"                             // 这一句是为了导入__FPU_PRESENT宏
#include "arm_math.h"
#else
#warning "CMSIS-DSP library is not available."
#endif
#else
#error "Unsupported compiler, please use GCC or Clang."
#endif

#endif  // EC_LIB_HAL_WRAPPER_DSP_H

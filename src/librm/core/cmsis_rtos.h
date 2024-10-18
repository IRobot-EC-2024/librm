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
 * @file  librm/core/cmsis_rtos.h
 * @brief 检测环境里有没有 CMSIS-RTOS
 */

#ifndef LIBRM_CORE_CMSIS_RTOS_H
#define LIBRM_CORE_CMSIS_RTOS_H

#if defined(__GNUC__) || defined(__clang__)
#if __has_include("cmsis_os.h")
#define LIBRM_USE_CMSIS_RTOS
#include "cmsis_os.h"
#elif __has_include("cmsis_os2.h")
#define LIBRM_USE_CMSIS_RTOS
#include "cmsis_os2.h"
#endif
#else
#error "Unsupported compiler, please use GCC or Clang."
#endif

#endif  // LIBRM_CORE_FREERTOS_H

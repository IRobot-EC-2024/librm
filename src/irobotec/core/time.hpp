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
 * @file  irobotec/core/time.hpp
 * @brief 时间模块，用来包装不同平台的时间相关功能
 */

#ifndef IROBOTEC_CORE_TIME_HPP
#define IROBOTEC_CORE_TIME_HPP

#include <chrono>
#include <thread>

#if defined(IROBOTEC_PLATFORM_STM32)
#include "irobotec/modules/freertos.h"
#include "irobotec/hal/stm32/hal.h"
#endif
#include "irobotec/core/typedefs.h"

namespace irobot_ec::core::time {

// STM32平台的延时比较复杂，所以专门实现两个函数
#if defined(IROBOTEC_PLATFORM_STM32)
/**
 * @brief 给STM32平台使用的延时函数
 * @param ms 延时时间，单位为毫秒
 */
inline void SleepMs(u32 ms) {
#ifdef IROBOTEC_USE_FREERTOS
  if (__get_IPSR()) {  // 检测当前是否在中断里，如果在中断里则调用HAL_Delay，否则调用osDelay
    HAL_Delay(ms);
  } else {
    osDelay(ms);
  }
#else
  HAL_Delay(ms);
#endif
}

/**
 * @brief 给STM32平台使用的延时函数
 * @param us 延时时间，单位为微秒
 */
inline void SleepUs(u32 us) {
  u32 ticks = 0;
  u32 t_old = 0;
  u32 t_now = 0;
  u32 t_cnt = 0;
  u32 reload = 0;
  reload = SysTick->LOAD;
  ticks = us * HAL_RCC_GetHCLKFreq() / 1000000;
  t_old = SysTick->VAL;
  while (true) {
    t_now = SysTick->VAL;
    if (t_now != t_old) {
      if (t_now < t_old) {
        t_cnt += t_old - t_now;
      } else {
        t_cnt += reload - t_now + t_old;
      }
      t_old = t_now;
      if (t_cnt >= ticks) {
        break;
      }
    }
  }
}
#endif

using namespace std::chrono_literals;

template <typename T>
concept delay_chrono_literal =
    std::is_same_v<T, std::chrono::seconds> || std::is_same_v<T, std::chrono::milliseconds> ||
    std::is_same_v<T, std::chrono::microseconds> || std::is_same_v<T, std::chrono::nanoseconds>;

/**
 * @tparam T        chrono_literals
 * @param  duration 延时时间
 */
template <typename T>
  requires delay_chrono_literal<T>
void Sleep(T duration) {
#if defined(IROBOTEC_PLATFORM_STM32)
  if constexpr (std::is_same_v<T, std::chrono::seconds>) {
    SleepMs(duration.count() * 1000);
  } else if constexpr (std::is_same_v<T, std::chrono::milliseconds>) {
    SleepMs(duration.count());
  } else if constexpr (std::is_same_v<T, std::chrono::microseconds>) {
    SleepUs(duration.count());
  } else if constexpr (std::is_same_v<T, std::chrono::nanoseconds>) {
    SleepUs(duration.count() / 1000);
  }
#else
  std::this_thread::sleep_for(duration);
#endif
}
}  // namespace irobot_ec::core::time

#endif  // IROBOTEC_CORE_TIME_HPP

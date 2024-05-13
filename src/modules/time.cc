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
 * @file  modules/time.cc
 * @brief 时间模块
 */

#include "modules/time.h"
#include "modules/freertos.h"
#include "hal/hal.h"

namespace irobot_ec::modules::time {

void SleepMs(u32 ms) {
#ifdef EC_LIB_USE_FREERTOS
  if (__get_IPSR()) {  // 检测当前是否在中断里，如果在中断里则调用HAL_Delay，否则调用osDelay
    HAL_Delay(ms);
  } else {
    osDelay(ms);
  }
#else
  HAL_Delay(ms);
#endif
}

void SleepUs(u32 us) {
  u32 ticks = 0;
  u32 t_old = 0;
  u32 t_now = 0;
  u32 t_cnt = 0;
  u32 reload = 0;
  reload = SysTick->LOAD;
  ticks = us * HAL_RCC_GetSysClockFreq();
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

}  // namespace irobot_ec::modules::time
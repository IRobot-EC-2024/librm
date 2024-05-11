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
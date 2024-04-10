/**
 * @file  bsp/ycmc_h7a/bsp_delay.h
 * @brief 利用systick实现的精确延时
 */

#include "hal/hal.h"
#if defined(STM32H723xx)

#include "bsp_delay.h"

namespace irobot_ec::bsp::ycmc_h7a {

/**
 * @brief 延时微秒
 * @param us 微秒
 */
void Delay::DelayUs(u32 us) {
  u32 ticks = 0;
  u32 t_old = 0;
  u32 t_now = 0;
  u32 t_cnt = 0;
  u32 reload = 0;
  reload = SysTick->LOAD;
  ticks = us * 280;
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

/**
 * @brief 延时毫秒
 * @param ms 毫秒
 */
void Delay::DelayMs(u32 ms) {
  while (ms--) {
    this->DelayUs(1000);
  }
}

/**
 * @brief 获取单例
 * @return 单例
 */
Delay &Delay::GetInstance() {
  static Delay instance;
  return instance;
}

}  // namespace irobot_ec::bsp::ycmc_h7a

#endif

#include "hal_wrapper/hal.h"

#if defined(STM32F407xx)

#include "bsp_delay.h"

namespace irobot_ec::bsp::dji_devboard_c {

/**
 * @brief 延时微秒
 * @param us 微秒
 */
void Delay::DelayUs(uint32_t us) {
  uint32_t ticks = 0;
  uint32_t t_old = 0;
  uint32_t t_now = 0;
  uint32_t t_cnt = 0;
  uint32_t reload = 0;
  reload = SysTick->LOAD;
  ticks = us * 168;
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
void Delay::DelayMs(uint32_t ms) {
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

}  // namespace irobot_ec::bsp::dji_devboard_c

#endif
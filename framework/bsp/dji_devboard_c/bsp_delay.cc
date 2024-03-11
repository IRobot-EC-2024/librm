
#include "hal_wrapper/hal_config.h"

#if defined(HAL_TIM_MODULE_ENABLED) && defined(STM32F407xx)

#include "bsp_delay.h"


using namespace bsp::dji_devboard_c;


static uint8_t fac_us = 0;
static uint32_t fac_ms = 0;


void delayInit() {
    fac_us = SystemCoreClock / 1000000;
    fac_ms = SystemCoreClock / 1000;
}


void delayUs(uint16_t us) {
    uint32_t ticks = 0;
    uint32_t t_old = 0;
    uint32_t t_now = 0;
    uint32_t t_cnt = 0;
    uint32_t reload = 0;
    reload = SysTick->LOAD;
    ticks = us * fac_us;
    t_old = SysTick->VAL;
    while (1) {
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


void delayMs(uint16_t ms) {
    uint32_t ticks = 0;
    uint32_t t_old = 0;
    uint32_t t_now = 0;
    uint32_t t_cnt = 0;
    uint32_t reload = 0;
    reload = SysTick->LOAD;
    ticks = ms * fac_ms;
    t_old = SysTick->VAL;
    while (1) {
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

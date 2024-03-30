/**
 * @file    bsp/bsp.cc
 * @brief   bsp工厂
 */

#include "bsp.h"

namespace irobot_ec::bsp {

/**
 * @brief   获取Delay实现
 * @return  DelayInterface&
 */
DelayInterface &BspFactory::GetDelay() {
#if defined(STM32F407xx)  // 大疆c板
  return dji_devboard_c::Delay::GetInstance();
#elif defined(STM32H723xx)  // YCMC-H7A
  return ycmc_h7a::Delay::GetInstance();
#else
  return nullptr;
#endif
}

}  // namespace irobot_ec::bsp
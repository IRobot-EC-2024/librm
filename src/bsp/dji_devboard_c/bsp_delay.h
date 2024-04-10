/**
 * @file  bsp/dji_devboard_c/bsp_delay.h
 * @brief 利用systick实现的精确延时
 */

#ifndef EC_LIB_DJI_C_BSP_DELAY_H
#define EC_LIB_DJI_C_BSP_DELAY_H

#include "hal_wrapper/hal.h"

#if defined(STM32F407xx)

#include "bsp/interface/bsp_delay_interface.h"
#include "modules/typedefs.h"

namespace irobot_ec::bsp::dji_devboard_c {

/**
 * @brief 延时功能实现
 * @note  单例类，不能实例化，使用时通过bsp工厂获取实例
 */
class Delay : public DelayInterface {
 public:
  // 禁止实例化
  Delay(const Delay &) = delete;
  Delay &operator=(const Delay &) = delete;

  void DelayUs(u32 us) override;
  void DelayMs(u32 ms) override;

  static Delay &GetInstance();

 private:
  Delay() = default;
  ~Delay() = default;
};

}  // namespace irobot_ec::bsp::dji_devboard_c

#endif

#endif  // EC_LIB_DJI_C_BSP_DELAY_H

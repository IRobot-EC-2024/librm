/**
 * @file    bsp/bsp.h
 * @brief   bsp工厂
 * @note    最终写具体的车的代码的时候可能用不到这里的工厂，直接调用具体的实现即可。
 *          这里提供一个统一的接口主要是为了让框架内部的一些模块通过这个接口获取不同平台的bsp实现。
 *          例如，BMI088等传感器的驱动需要延时功能，那么可以直接通过BspFactory::GetDelay()获取不同平台的延时实现。
 *          而不需要在模块内部写一堆的if else来判断当前平台是什么，然后选择不同的延时实现。
 */

#ifndef EC_LIB_BSP_BSP_FACTORY_H_
#define EC_LIB_BSP_BSP_FACTORY_H_

// 导入bsp接口
#include "interface/bsp_delay_interface.h"
#include "interface/bsp_ins_interface.h"

// 根据宏定义导入对应平台的bsp实现
#include "hal_wrapper/hal.h"
#if defined(STM32F407xx)
#include "bsp/dji_devboard_c/bsp_delay.h"
#include "bsp/dji_devboard_c/bsp_ins.h"
#include "bsp/dji_devboard_c/bsp_can.h"
#elif defined(STM32H723xx)
#include "bsp/ycmc_h7a/bsp_delay.h"
#endif

// 导入通用的bsp实现
#include "common/bsp_uart.h"

namespace irobot_ec::bsp {

/**
 * @brief bsp工厂，用于获取不同的bsp实现
 */
class BspFactory {
 public:
  // 禁止实例化
  BspFactory() = delete;

  static DelayInterface &GetDelay();
  static CanInterface &GetCan();

 private:
  ~BspFactory() = default;
};

}  // namespace irobot_ec::bsp

#endif  // EC_LIB_BSP_BSP_FACTORY_H_

/**
 * @file    bsp/bsp.h
 * @brief   bsp工厂
 */

#ifndef EC_LIB_BSP_BSP_FACTORY_H_
#define EC_LIB_BSP_BSP_FACTORY_H_

// 导入bsp接口
#include "interface/bsp_delay_interface.h"

// 根据宏定义导入不同的bsp实现
#include "hal_wrapper/hal_config.h"
#ifdef STM32F407xx
#include "bsp/dji_devboard_c/bsp_delay.h"
#endif

namespace irobot_ec::bsp {

/**
 * @brief bsp工厂，用于获取不同的bsp实现
 */
class BspFactory {
 public:
  // 禁止实例化
  BspFactory() = delete;

  static DelayInterface &GetDelay();

 private:
  ~BspFactory() = default;
};

}  // namespace irobot_ec::bsp

#endif  // EC_LIB_BSP_BSP_FACTORY_H_

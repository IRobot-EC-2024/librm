/**
 * @file    bsp/interface/bsp_delay_interface.h
 * @brief   延时功能接口
 */

#ifndef EC_LIB_BSP_INTERFACE_BSP_DELAY_INTERFACE_H_
#define EC_LIB_BSP_INTERFACE_BSP_DELAY_INTERFACE_H_

#include "modules/typedefs.h"

namespace irobot_ec::bsp {

/**
 * @brief 延时功能接口
 */
class DelayInterface {
 public:
  /**
   * @brief 延时微秒
   * @param us  微秒
   */
  virtual void DelayUs(u32 us) = 0;
  /**
   * @brief 延时毫秒
   * @param ms  毫秒
   */
  virtual void DelayMs(u32 ms) = 0;
};

}  // namespace irobot_ec::bsp

#endif  // EC_LIB_BSP_INTERFACE_BSP_DELAY_INTERFACE_H_

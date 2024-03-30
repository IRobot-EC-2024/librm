/**
 * @file    bsp/interface/bsp_ins_interface.h
 * @brief   惯导功能接口
 */

#ifndef EC_LIB_BSP_INTERFACE_BSP_INS_INTERFACE_H_
#define EC_LIB_BSP_INTERFACE_BSP_INS_INTERFACE_H_

#include <array>

#include "modules/typedefs.h"

namespace irobot_ec::bsp {

/**
 * @brief 惯导功能接口
 */
class InsInterface {
 public:
  /**
   * @brief     获取加速度计数据
   * @return    加速度
   */
  virtual std::array<fp32, 3> &acc() = 0;
  /**
   * @brief     获取陀螺仪数据
   * @return    角速度
   */
  virtual std::array<fp32, 3> &gyro() = 0;
  /**
   * @brief     获取磁力计数据
   * @return    磁场
   */
  virtual std::array<fp32, 3> &mag() = 0;
  /**
   * @brief     获取欧拉角数据
   * @return    欧拉角(yaw, pitch, roll)
   */
  virtual std::array<fp32, 3> &ypr() = 0;
  /**
   * @brief     获取四元数数据
   * @return    四元数(w, x, y, z)
   */
  virtual std::array<fp32, 4> &quaternion() = 0;
  /**
   * @brief     更新数据
   */
  virtual void Update() = 0;
};

}  // namespace irobot_ec::bsp

#endif  // EC_LIB_BSP_INTERFACE_BSP_INS_INTERFACE_H_

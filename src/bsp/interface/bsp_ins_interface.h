/**
 * @file    bsp/interface/bsp_ins_interface.h
 * @brief   惯导功能接口
 */

#ifndef EC_LIB_BSP_INTERFACE_BSP_INS_INTERFACE_H_
#define EC_LIB_BSP_INTERFACE_BSP_INS_INTERFACE_H_

#include "modules/typedefs.h"

namespace irobot_ec::bsp {

/**
 * @brief 惯导功能接口
 */
class InsInterface {
 public:
  virtual ~InsInterface() = default;
  /**
   * @brief     获取加速度计数据
   * @return    加速度(m/s^2)
   */
  virtual f32 acc_x() const = 0;
  virtual f32 acc_y() const = 0;
  virtual f32 acc_z() const = 0;
  /**
   * @brief     获取陀螺仪数据
   * @return    角速度(rad/s)
   */
  virtual f32 gyro_x() const = 0;
  virtual f32 gyro_y() const = 0;
  virtual f32 gyro_z() const = 0;
  /**
   * @brief     获取磁力计数据
   * @return    磁场(uT)
   */
  virtual f32 mag_x() const = 0;
  virtual f32 mag_y() const = 0;
  virtual f32 mag_z() const = 0;
  /**
   * @brief     获取欧拉角数据
   * @return    欧拉角(yaw, pitch, roll)
   */
  virtual f32 yaw() const = 0;
  virtual f32 pitch() const = 0;
  virtual f32 roll() const = 0;
  /**
   * @brief     获取四元数数据
   * @return    四元数(w, x, y, z)
   */
  virtual f32 quat_w() const = 0;
  virtual f32 quat_x() const = 0;
  virtual f32 quat_y() const = 0;
  virtual f32 quat_z() const = 0;
  /**
   * @brief     更新数据
   */
  virtual void Update() = 0;
};

}  // namespace irobot_ec::bsp

#endif  // EC_LIB_BSP_INTERFACE_BSP_INS_INTERFACE_H_

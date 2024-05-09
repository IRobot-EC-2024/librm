/**
 * @file    hal/can_interface.h
 * @brief   CAN接口，bxCAN和fdCAN的基类
 */

#ifndef EC_LIB_HAL_CAN_INTERFACE_H
#define EC_LIB_HAL_CAN_INTERFACE_H

#include "modules/typedefs.h"

namespace irobot_ec::device {
class CanDeviceBase;
}

namespace irobot_ec::hal {

struct CanRxMsg {
  u8 data[8];
  u32 rx_std_id;
  u32 dlc;
};

/**
 * @brief CAN外设基类
 * @note  借助CanDeviceBase类使用观察者模式实现回调机制
 */
class CanBase {
  friend class device::CanDeviceBase;

 public:
  virtual ~CanBase() = default;

  /**
   * @brief 向总线上发送数据
   * @param id      数据帧ID
   * @param data    数据指针
   * @param size    数据长度
   */
  virtual void Write(u16 id, const u8 *data, usize size) = 0;

  /**
   * @brief 设置过滤器
   * @param id
   * @param mask
   */
  virtual void SetFilter(u16 id, u16 mask) = 0;

  /**
   * @brief 启动CAN外设
   */
  virtual void Begin() = 0;

  /**
   * @brief 停止CAN外设
   */
  virtual void Stop() = 0;

 protected:
  /**
   * @brief 注册CAN设备
   * @param device 设备对象
   */
  virtual void RegisterDevice(device::CanDeviceBase &device, u32 rx_stdid) = 0;
};

}  // namespace irobot_ec::hal

#endif  // EC_LIB_HAL_CAN_INTERFACE_H

/*
  Copyright (c) 2024 XDU-IRobot

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
*/

/**
 * @file    hal/can_interface.h
 * @brief   bxCAN和fdCAN的接口类
 */

#ifndef EC_LIB_HAL_CAN_INTERFACE_H
#define EC_LIB_HAL_CAN_INTERFACE_H

#include "modules/typedefs.h"

#include <array>

namespace irobot_ec::device {
class CanDeviceBase;
}

namespace irobot_ec::hal {

struct CanMsg {
  std::array<u8, 8> data;
  u32 rx_std_id;
  u32 dlc;
};

enum class CanTxPriority {
  kLow,
  kNormal,
  kHigh,
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
   * @brief 立即向总线上发送数据
   * @param id      数据帧ID
   * @param data    数据指针
   * @param size    数据长度
   */
  virtual void Write(u16 id, const u8 *data, usize size) = 0;

  /***
   * @brief 从消息队列里取出一条消息发送
   */
  virtual void Write() = 0;

  /**
   * @brief 向消息队列里加入一条消息
   * @param id        数据帧ID
   * @param data      数据指针
   * @param size      数据长度
   * @param priority  消息的优先级
   */
  virtual void Enqueue(u16 id, const u8 *data, usize size, CanTxPriority priority /*=CanTxPriority::kNormal*/) = 0;

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

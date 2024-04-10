/**
 * @file  components/rc/dr16.cc
 * @brief DR16接收机
 */

#include "hal/hal.h"
#if defined(HAL_UART_MODULE_ENABLED)

#include "dr16.h"

namespace irobot_ec::components {

/**
 * @param uart UART对象
 */
DR16::DR16(bsp::Uart &uart) : uart_(&uart) {
  static bsp::UartCallbackFunction rx_callback =
      std::bind(&DR16::RxCallback, this, std::placeholders::_1, std::placeholders::_2);
  this->uart_->AttachRxCallback(rx_callback);
}

/**
 * @brief 开始接收遥控器数据
 */
void DR16::StartReceive() { this->uart_->StartReceive(); }

/**
 * @brief 串口接收完成中断回调函数
 * @param data      接收到的数据
 * @param rx_len    接收到的数据长度
 */
void DR16::RxCallback(const std::vector<u8> &data, u16 rx_len) {
  // 长度不等于18说明接收不完整，丢弃这一帧
  if (rx_len != 18) {
    return;
  }
  this->axes_[0] = (data[0] | (data[1] << 8)) & 0x07ff;         //!< Channel 0
  this->axes_[1] = ((data[1] >> 3) | (data[2] << 5)) & 0x07ff;  //!< Channel 1
  this->axes_[2] = ((data[2] >> 6) | (data[3] << 2) |           //!< Channel 2
                    (data[4] << 10)) &
                   0x07ff;
  this->axes_[3] = ((data[4] >> 1) | (data[5] << 7)) & 0x07ff;                      //!< Channel 3
  this->switches_[0] = static_cast<RcSwitchState>((data[5] >> 4) & 0x0003);         //!< Switch left
  this->switches_[1] = static_cast<RcSwitchState>(((data[5] >> 4) & 0x000C) >> 2);  //!< Switch right
  this->mouse_[0] = data[6] | (data[7] << 8);                                       //!< Mouse X axis
  this->mouse_[1] = data[8] | (data[9] << 8);                                       //!< Mouse Y axis
  this->mouse_[2] = data[10] | (data[11] << 8);                                     //!< Mouse Z axis
  this->mouse_button_[0] = data[12];                                                //!< Mouse Left Is Press ?
  this->mouse_button_[1] = data[13];                                                //!< Mouse Right Is Press ?
  this->keyboard_key_ = data[14] | (data[15] << 8);                                 //!< KeyBoard value
  this->axes_[4] = data[16] | (data[17] << 8);                                      // NULL

  this->axes_[0] -= 1024;
  this->axes_[1] -= 1024;
  this->axes_[2] -= 1024;
  this->axes_[3] -= 1024;
  this->axes_[4] -= 1024;
}

i16 DR16::left_x() const { return this->axes_[2]; }
i16 DR16::left_y() const { return this->axes_[3]; }
i16 DR16::right_x() const { return this->axes_[0]; }
i16 DR16::right_y() const { return this->axes_[1]; }
i16 DR16::dial() const { return this->axes_[4]; }
RcSwitchState DR16::switch_l() const { return this->switches_[1]; }
RcSwitchState DR16::switch_r() const { return this->switches_[0]; }
bool DR16::key(RcKey key) const { return (this->keyboard_key_ & static_cast<u16>(key)); }

}  // namespace irobot_ec::components

#endif
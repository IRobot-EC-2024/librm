
#include "hal/hal.h"
#if defined(USE_HAL_UART_REGISTER_CALLBACKS)
#if (USE_HAL_UART_REGISTER_CALLBACKS != 1u)
#error "UART register callback must be enabled!"  // 必须启用UART的Register callback
#endif
#endif
#if defined(HAL_UART_MODULE_ENABLED)

#include "bsp_uart.h"

#include <functional>

#include "modules/exception/exception.h"

using irobot_ec::modules::exception::Exception;
using irobot_ec::modules::exception::ThrowException;

/**
 * @brief  把std::function转换为函数指针
 * @tparam CallbackFunctionType 回调函数类型
 * @tparam HandleType           外设句柄类型
 * @param  fn                   要转换的函数
 * @return                      转换后的函数指针
 * @note
 * 背景：因为要用面向对象的方式对外设进行封装，所以回调函数必须存在于类内。但是存在于类内就意味着这个回调函数多了一个this参数，
 * 而HAL库要求的回调函数并没有这个this参数。通过std::bind，可以生成一个参数列表里没有this指针的std::function对象，而std::function
 * 并不能直接强转成函数指针。借助这个函数，可以把std::function对象转换成函数指针。然后就可以把这个类内的回调函数传给HAL库了。
 */
template <typename CallbackFunctionType, typename HandleType>
static CallbackFunctionType StdFunctionToCallbackFunctionPtr(std::function<void()> fn) {
  static auto fn_v = std::move(fn);
  return [](HandleType *dummy1, u16 dummy2) { fn_v(); };
}

namespace irobot_ec::bsp {

/**
 * @param huart     HAL库的UART句柄
 * @param rx_size   接收缓冲区大小
 * @param mode      UART工作模式（正常、中断、DMA）
 */
Uart::Uart(UART_HandleTypeDef &huart, usize rx_size, UartMode tx_mode, UartMode rx_mode)
    : huart_(&huart),
      tx_mode_(tx_mode),
      rx_mode_(rx_mode),
      rx_buf_{std::vector<u8>(rx_size), std::vector<u8>(rx_size)} {
  // 注册接收完成回调函数
  HAL_UART_RegisterRxEventCallback(&huart,
                                   StdFunctionToCallbackFunctionPtr<pUART_RxEventCallbackTypeDef, UART_HandleTypeDef>(
                                       std::bind(&Uart::HalRxCpltCallback, this)));
  // 检查dma模式下是否已经配置好DMA
  if (tx_mode == UartMode::kDma && this->huart_->hdmatx == nullptr) {
    ThrowException(Exception::kHALError);
  }
  if (rx_mode == UartMode::kDma && this->huart_->hdmarx == nullptr) {
    ThrowException(Exception::kHALError);
  }
}

/**
 * @brief 留给用户定义的接收回调函数，可以在子类中重写实现想要的功能
 */
void Uart::RxCallback() {}

/**
 * @brief 发送数据
 * @param data 数据指针
 * @param size 数据大小
 */
void Uart::Write(const u8 *data, usize size) {
  switch (this->tx_mode_) {
    case UartMode::kNormal:
      HAL_UART_Transmit(this->huart_, const_cast<u8 *>(data), size, HAL_MAX_DELAY);
      break;
    case UartMode::kInterrupt:
      HAL_UART_Transmit_IT(this->huart_, const_cast<u8 *>(data), size);
      break;
#if defined(HAL_DMA_MODULE_ENABLED)
    case UartMode::kDma:
      HAL_UART_Transmit_DMA(this->huart_, const_cast<u8 *>(data), size);
      break;
#endif
  }
}

/**
 * @brief 开始接收数据
 */
void Uart::StartReceive() {
  switch (this->rx_mode_) {
    case UartMode::kNormal:
      HAL_UART_Receive(this->huart_, this->rx_buf_[0].data(), this->rx_buf_[this->buffer_selector_].size(),
                       HAL_MAX_DELAY);
      break;
    case UartMode::kInterrupt:
      HAL_UART_Receive_IT(this->huart_, this->rx_buf_[0].data(), this->rx_buf_[this->buffer_selector_].size());
      break;
#if defined(HAL_DMA_MODULE_ENABLED)
    case UartMode::kDma:
      HAL_UARTEx_ReceiveToIdle_DMA(this->huart_, this->rx_buf_[0].data(), this->rx_buf_[this->buffer_selector_].size());
      __HAL_DMA_DISABLE_IT(this->huart_->hdmarx, DMA_IT_HT);  // 关闭DMA半传输中断
      break;
#endif
  }
}

const std::vector<u8> &Uart::rx_buffer() const { return rx_buf_[buffer_selector_]; }

/**
 * @brief 接收完成回调函数
 * @note  这个函数会在HAL库的回调函数中被调用
 */
void Uart::HalRxCpltCallback() {
  // 判断rx模式，重新启动接收
  switch (this->rx_mode_) {
    case UartMode::kNormal:
      HAL_UART_Receive(this->huart_, this->rx_buf_[0].data(), this->rx_buf_[!this->buffer_selector_].size(),
                       HAL_MAX_DELAY);
      break;
    case UartMode::kInterrupt:
      HAL_UART_Receive_IT(this->huart_, this->rx_buf_[0].data(), this->rx_buf_[!this->buffer_selector_].size());
      break;
#if defined(HAL_DMA_MODULE_ENABLED)
    case UartMode::kDma:
      HAL_UARTEx_ReceiveToIdle_DMA(this->huart_, this->rx_buf_[0].data(),
                                   this->rx_buf_[!this->buffer_selector_].size());
      __HAL_DMA_DISABLE_IT(this->huart_->hdmarx, DMA_IT_HT);  // 关闭DMA半传输中断
      break;
#endif
  }
  // 调用子类重写的回调函数
  this->RxCallback();
  // 切换缓冲区
  this->buffer_selector_ = !this->buffer_selector_;
}

}  // namespace irobot_ec::bsp

#endif
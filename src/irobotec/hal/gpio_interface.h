
#ifndef IROBOTEC_HAL_GPIO_INTERFACE_H
#define IROBOTEC_HAL_GPIO_INTERFACE_H

namespace irobot_ec::hal {

/**
 * @brief GPIO引脚接口类
 */
class PinInterface {
 public:
  virtual ~PinInterface() = default;

  virtual void Write(bool state) = 0;

  [[nodiscard]] virtual bool Read() const = 0;
};

}  // namespace irobot_ec::hal

#endif  // IROBOTEC_HAL_GPIO_INTERFACE_H

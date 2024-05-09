
#include "device.h"

namespace irobot_ec::device {

/**
 * @return 设备在线状态
 */
DeviceStatus Device::GetDeviceStatus() const { return this->status_; }

}  // namespace irobot_ec::device
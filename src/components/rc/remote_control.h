/**
 * @file  components/rc/remote_control.h
 * @brief 遥控器的类封装
 * @todo  未完成
 */

#ifndef EC_LIB_COMPONENTS_REMOTE_CONTROL_H
#define EC_LIB_COMPONENTS_REMOTE_CONTROL_H

namespace irobot_ec::components::remote_control {

typedef enum {
  RC_SWITCH_UP = 1,
  RC_SWITCH_DOWN,
  RC_SWITCH_MID,
} rc_switch_e;

}  // namespace irobot_ec::components::remote_control

#endif  // EC_LIB_COMPONENTS_REMOTE_CONTROL_H

/* EOF */

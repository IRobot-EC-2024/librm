/***
 * @file examples/gm6020.cc
 * @brief 例程：GM6020电机位置闭环控制
 */

#include "can.h"
#include "cmsis_os.h"
#include "components/motor/dji/dji_motor.h"
#include "modules/algorithm/pid/pid.h"

using components::motor::GM6020;
using modules::algorithm::PID::PIDType;
using modules::algorithm::PID::RingPID;

extern "C" {

[[noreturn]] void ExampleGM6020Task(const void *pv_arg) {
  UNUSED(pv_arg);

  GM6020 gm6020(&hcan1, 1);
  RingPID ringPID(PIDType::kPosition, 5, 0, 0, 30000, 0, 8191);

  for (;;) {
    osDelay(1);
    ringPID.update(3000, gm6020.encoder());
    gm6020.SetCurrent(static_cast<int16_t>(ringPID.value()));
    gm6020.PushControlMessage();
  }
}
}

/***
 * @file examples/gm6020.cpp
 * @brief 例程：GM6020电机位置闭环控制
 */

#include "cmsis_os.h"
#include "can.h"

#include "modules/algorithm/pid/pid.h"
#include "components/motor/dji/dji_motor.h"


using components::motor::GM6020;
using modules::algorithm::PID::RingPID;
using modules::algorithm::PID::PIDType;


GM6020 gm6020(&hcan1, 4);
RingPID ringPID(PIDType::kPosition,
                5, 0, 0,
                30000, 0, 8191);


#ifdef __cplusplus
extern "C" {
#endif

[[noreturn]] void chassisTask(const void *pvArgument) {

    UNUSED(pvArgument);

    for(;;) {
        osDelay(1);
        ringPID.update(3000, gm6020.getEncoder());
        gm6020.setCurrent(static_cast<int16_t>(ringPID.value()));
    }
}

#ifdef __cplusplus
}
#endif



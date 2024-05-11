/**
 * @file  modules/time.h
 * @brief 时间模块
 */

#ifndef EC_LIB_TIME_H
#define EC_LIB_TIME_H

#include "modules/typedefs.h"

namespace irobot_ec::modules::time {

void SleepMs(u32 ms);
void SleepUs(u32 us);

}  // namespace irobot_ec::modules::time

#endif  // EC_LIB_TIME_H

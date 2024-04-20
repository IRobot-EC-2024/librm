/**
 * @file  hal/fdcan.cc
 * @brief fdCAN类库
 * @todo  未完成
 */

#include "hal/hal.h"
#if defined(HAL_FDCAN_MODULE_ENABLED)
#if defined(USE_HAL_FDCAN_REGISTER_CALLBACKS)
#if (USE_HAL_FDCAN_REGISTER_CALLBACKS != 1u)
#error "FDCAN register callback must be enabled!"
#endif
#endif

#include "fdcan.h"

#endif
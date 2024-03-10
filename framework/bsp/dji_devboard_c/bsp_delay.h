
#pragma once

#ifndef RMFRAME_BSP_DELAY_H
#define RMFRAME_BSP_DELAY_H


#include "modules/typedefs.h"


namespace bsp::delay {

    void delayUs(uint16_t us);

    void delayMs(uint16_t ms);

}   // namespace bsp::delay


#endif //RMFRAME_BSP_DELAY_H


#pragma once

#ifndef EC_LIB_DJI_C_BSP_LED_H
#define EC_LIB_DJI_C_BSP_LED_H

// check if target chip is STM32F4 series
#if defined(STM32F407xx) && defined(TIM_MODULE_ENABLED)

    #include "tim.h"

    namespace bsp::LED {

            class LED {
            public:
                LED(TIM_HandleTypeDef htim);
                LED()  = delete;
                ~LED() = default;

                void on();
                void off();
            private:
                GPIO_TypeDef* GPIOx;
                uint16_t GPIO_Pin;
            };

    }   // namespace bsp::LED

#endif

#endif // EC_LIB_DJI_C_BSP_LED_H

/* EOF */


#pragma once

#ifndef EC_LIB_COMPONENTS_MOTOR_UNITREE_MOTOR_H
#define EC_LIB_COMPONENTS_MOTOR_UNITREE_MOTOR_H

#if defined(USART_MODULE_ENABLED)

    #include "typedefs.h"

    namespace Compnenents::Motor {

        enum class UnitreeMotorMode {
            kLock = 0x00,
            kFOC = 0x01,
            kCalibration = 0x02,
            kReserved1 = 0x03,
            kReserved2 = 0x04,
            kReserved3 = 0x05,
            kReserved4 = 0x06,
            kReserved5 = 0x07,
        };

        class UnitreeMotorBase {

        public:
            UnitreeMotorBase() = delete;
            ~UnitreeMotorBase() = default;

        protected:
            uint8_t tx_buffer_[17];
            uint16_t id_{};

            /** MOTOR FEEDBACK DATA **/
            UnitreeMotorMode mode_{};
            int16_t tau_{};         // Divide by 256 to get the real value(N*m)
            int16_t velocity_{};    // Divide by 1000 to get the real value(rad/s)

            /*************************/
        };

    }

#endif

#endif // EC_LIB_COMPONENTS_MOTOR_UNITREE_MOTOR_H

/* EOF */


#pragma once

#ifndef BALANCE_CHASSIS_EXCEPTIONS_H
#define BALANCE_CHASSIS_EXCEPTIONS_H

namespace modules::exceptions {

    enum class Exception {
        kException,

        kHALError,
        kValueError,
        kArithmeticError,
    };

    void throwException(Exception e);

}   // namespace Modules::Exceptions





#endif


#pragma once

#ifndef EC_LIB_MODULES_EXCEPTIONS_H
#define EC_LIB_MODULES_EXCEPTIONS_H

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


#ifndef EC_LIB_MODULES_EXCEPTIONS_H
#define EC_LIB_MODULES_EXCEPTIONS_H

namespace modules::exceptions {

enum class Exception {
  kException,

  kTypeError,
  kHALError,
  kValueError,
  kArithmeticError,
};

void ThrowException(Exception e);

}  // namespace modules::exceptions

#endif


#ifndef EC_LIB_MODULES_EXCEPTIONS_H
#define EC_LIB_MODULES_EXCEPTIONS_H

namespace irobot_ec::modules::exceptions {

/**
 * @brief 异常类型
 */
enum class Exception {
  kException,

  kTypeError,
  kHALError,
  kValueError,
  kArithmeticError,
};

void ThrowException(Exception e);

}  // namespace irobot_ec::modules::exceptions

#endif

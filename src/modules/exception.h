/**
 * @file  modules/exception/exception.cc
 * @brief 异常处理
 */

#ifndef EC_LIB_MODULES_EXCEPTIONS_H
#define EC_LIB_MODULES_EXCEPTIONS_H

namespace irobot_ec::modules::exception {

/**
 * @brief 异常类型
 */
enum class Exception {
  kException,

  kTypeError,
  kKeyError,
  kHALError,
  kValueError,
};

void ThrowException(Exception e);

}  // namespace irobot_ec::modules::exception

#endif

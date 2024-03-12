
/***
 * @file  modules/algorithm/utils/utils.hpp
 * @brief 一些常用的工具函数
 */

#ifndef EC_LIB_MODULES_ALGORITHM_UTILS_HPP
#define EC_LIB_MODULES_ALGORITHM_UTILS_HPP

#include <cmath>

#include "modules/typedefs.h"

namespace modules::algorithm::utils {

/***
 * @brief Sign function
 * @tparam T Type of value
 * @param value Input value
 * @return 1 if value > 0, -1 if value < 0, 0 if value == 0
 */
template <typename T>
int sign(const T value) {
  if (value > 0) {
    return 1;
  } else if (value < 0) {
    return -1;
  } else {
    return 0;
  }
}

/***
 * @brief Deadline limit
 * @tparam T Type of value
 * @param value Input value
 * @param min_value Minimum value
 * @param max_value Maximum value
 * @return value if it is in range of [min_value, max_value], otherwise 0
 */
template <typename T>
T deadline(T value, T min_value, T max_value) {
  if (value < min_value || value > max_value) {
    return 0;
  } else {
    return value;
  }
}

/***
 * @brief 把输入值限制在一个范围内
 * @tparam T Type of value
 * @param input Input value
 * @param min_value Minimum value
 * @param max_value Maximum value
 * @return value if it is in range of [min_value, max_value], otherwise the
 * nearest limit
 */
template <typename T>
T constrain(T input, T min_value, T max_value) {
  if (input < min_value) {
    return min_value;
  } else if (input > max_value) {
    return max_value;
  } else {
    return input;
  }
}

/***
 * @brief   把输入值限制在一个周期范围内
 * @tparam  T           输入值的类型
 * @param   input       输入值
 * @param   min_value   周期下限
 * @param   max_value   周期上限
 * @return              若输入值超出周期范围，则返回限制后的值，否则返回原值
 */
template <typename T>
T loopConstrain(T input, T min_value, T max_value) {
  T cycle = max_value - min_value;
  if (cycle < 0) {
    return input;
  }

  if (input > max_value) {
    while (input > max_value) {
      input -= cycle;
    }
  } else if (input < min_value) {
    while (input < min_value) {
      input += cycle;
    }
  }
  return input;
}

/***
 * @brief   限制输入值的绝对值
 * @tparam  T           输入值的类型
 * @param   input       输入值
 * @param   max_value   限制的最大绝对值
 * @return              限制后的值
 */
template <typename T>
T absConstrain(T input, T max_value) {
  if (input > max_value) {
    return max_value;
  } else if (input < -max_value) {
    return -max_value;
  } else {
    return input;
  }
}

/***
 * @brief 角度转弧度
 * @param deg   角度值
 * @return      弧度值
 */
fp32 degToRad(fp32 deg);

/***
 * @brief 四元数转欧拉角
 * @param q         四元数
 * @param euler     欧拉角
 */
void quatToEuler(const fp32 q[4], fp32 euler[3]);

}  // namespace modules::algorithm::utils

#endif  // EC_LIB_MODULES_ALGORITHM_UTILS_HPP

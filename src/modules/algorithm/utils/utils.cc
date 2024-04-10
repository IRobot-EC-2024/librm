/**
 * @file  modules/algorithm/utils/utils.cc
 * @brief 通用工具函数
 */

#include "utils.hpp"

using namespace irobot_ec::modules::algorithm::utils;

f32 deadline(f32 value, f32 min_value, f32 max_value) {
  if (value < min_value || value > max_value) {
    return 0;
  } else {
    return value;
  }
}

f32 constrain(f32 input, f32 min_value, f32 max_value) {
  if (input < min_value) {
    return min_value;
  } else if (input > max_value) {
    return max_value;
  } else {
    return input;
  }
}

f32 loopConstrain(f32 input, f32 min_value, f32 max_value) {
  f32 cycle = max_value - min_value;
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

f32 degToRad(f32 deg) { return deg * M_PI / 180; }

void quatToEuler(const f32 q[4], f32 euler[3]) {
  euler[0] = atan2f(2 * (q[0] * q[1] + q[2] * q[3]), 1 - 2 * (q[1] * q[1] + q[2] * q[2]));
  euler[1] = asinf(2 * (q[0] * q[2] - q[3] * q[1]));
  euler[2] = atan2f(2 * (q[0] * q[3] + q[1] * q[2]), 1 - 2 * (q[2] * q[2] + q[3] * q[3]));
}

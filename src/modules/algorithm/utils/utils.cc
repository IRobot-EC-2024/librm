/**
 * @file  modules/algorithm/utils/utils.cc
 * @brief 通用工具函数
 */

#include "utils.hpp"

using namespace irobot_ec::modules::algorithm::utils;

fp32 deadline(fp32 value, fp32 min_value, fp32 max_value) {
  if (value < min_value || value > max_value) {
    return 0;
  } else {
    return value;
  }
}

fp32 constrain(fp32 input, fp32 min_value, fp32 max_value) {
  if (input < min_value) {
    return min_value;
  } else if (input > max_value) {
    return max_value;
  } else {
    return input;
  }
}

fp32 loopConstrain(fp32 input, fp32 min_value, fp32 max_value) {
  fp32 cycle = max_value - min_value;
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

fp32 degToRad(fp32 deg) { return deg * M_PI / 180; }

void quatToEuler(const fp32 q[4], fp32 euler[3]) {
  euler[0] = atan2f(2 * (q[0] * q[1] + q[2] * q[3]), 1 - 2 * (q[1] * q[1] + q[2] * q[2]));
  euler[1] = asinf(2 * (q[0] * q[2] - q[3] * q[1]));
  euler[2] = atan2f(2 * (q[0] * q[3] + q[1] * q[2]), 1 - 2 * (q[2] * q[2] + q[3] * q[3]));
}

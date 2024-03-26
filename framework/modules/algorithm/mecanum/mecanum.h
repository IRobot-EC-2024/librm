/**
 * @file  modules/algorithm/mecanum/mecanum.h
 * @brief 麦轮正运动学结算
 */

#ifndef EC_LIB_MODULES_ALGORITHM_MECANUM_MECANUM_H
#define EC_LIB_MODULES_ALGORITHM_MECANUM_MECANUM_H

#include <array>

#include "modules/typedefs.h"

namespace irobot_ec::modules::algorithm {

/**
 * @brief 麦轮正运动学结算
 */
class Mecanum {
 public:
  Mecanum() = delete;
  ~Mecanum() = default;
  Mecanum(fp32 wheel_base, fp32 wheel_track);

  // no copy
  Mecanum(const Mecanum &) = delete;
  Mecanum &operator=(const Mecanum &) = delete;

  void Calculate(fp32 vx, fp32 vy, fp32 wz);
  [[nodiscard]] fp32 v_lf() const;
  [[nodiscard]] fp32 v_rf() const;
  [[nodiscard]] fp32 v_lb() const;
  [[nodiscard]] fp32 v_rb() const;

 private:
  fp32 wheel_base_;   // 轮子间距
  fp32 wheel_track_;  // 轮子轴距
  fp32 speeds_[4];
};

}  // namespace irobot_ec::modules::algorithm

#endif  // EC_LIB_MODULES_ALGORITHM_MECANUM_MECANUM_H

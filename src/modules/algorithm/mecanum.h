/**
 * @file  modules/algorithm/mecanum/mecanum.h
 * @brief 麦轮正运动学解算
 */

#ifndef EC_LIB_MODULES_ALGORITHM_MECANUM_MECANUM_H
#define EC_LIB_MODULES_ALGORITHM_MECANUM_MECANUM_H

#include "modules/typedefs.h"

namespace irobot_ec::modules::algorithm {

/**
 * @brief 麦轮正运动学解算
 */
class Mecanum {
 public:
  Mecanum() = delete;
  ~Mecanum() = default;
  Mecanum(f32 wheel_base, f32 wheel_track);

  // no copy
  Mecanum(const Mecanum &) = delete;
  Mecanum &operator=(const Mecanum &) = delete;

  void Calculate(f32 vx, f32 vy, f32 wz);
  [[nodiscard]] f32 v_lf() const;
  [[nodiscard]] f32 v_rf() const;
  [[nodiscard]] f32 v_lb() const;
  [[nodiscard]] f32 v_rb() const;

 private:
  f32 wheel_base_;   // 轮子间距
  f32 wheel_track_;  // 轮子轴距
  f32 speeds_[4];
};

}  // namespace irobot_ec::modules::algorithm

#endif  // EC_LIB_MODULES_ALGORITHM_MECANUM_MECANUM_H

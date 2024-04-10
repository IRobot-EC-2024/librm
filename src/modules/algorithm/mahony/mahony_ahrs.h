/**
 * @file  modules/algorithm/mahony/mahony_ahrs.h
 * @brief Mahony姿态解算算法
 * @todo  利用DSP库加速运算
 */

#ifndef EC_LIB_MODULES_ALGORITHM_MAHONY_AHRS_H
#define EC_LIB_MODULES_ALGORITHM_MAHONY_AHRS_H

#include <array>

#include "modules/typedefs.h"

namespace irobot_ec::modules::algorithm {

class MahonyAhrs {
 public:
  MahonyAhrs(f32 sample_freq = 1000.0f, f32 kp = 1.0f, f32 ki = 0.0f);
  ~MahonyAhrs() = default;

  void Update(f32 gx, f32 gy, f32 gz, f32 ax, f32 ay, f32 az, f32 mx, f32 my, f32 mz);
  void UpdateImu(f32 gx, f32 gy, f32 gz, f32 ax, f32 ay, f32 az);
  [[nodiscard]] f32 quat_w() const;
  [[nodiscard]] f32 quat_x() const;
  [[nodiscard]] f32 quat_y() const;
  [[nodiscard]] f32 quat_z() const;
  [[nodiscard]] f32 euler_yaw() const;
  [[nodiscard]] f32 euler_pitch() const;
  [[nodiscard]] f32 euler_roll() const;

 private:
  f32 two_kp_, two_ki_;
  f32 q0_, q1_, q2_, q3_;
  f32 recip_norm_;
  f32 q0q0_, q0q1_, q0q2_, q0q3_, q1q1_, q1q2_, q1q3_, q2q2_, q2q3_, q3q3_;
  f32 hx_, hy_, bx_, bz_;
  f32 halfvx_, halfvy_, halfvz_, halfwx_, halfwy_, halfwz_;
  f32 halfex_, halfey_, halfez_;
  f32 qa_, qb_, qc_;
  f32 quaternion_[4];
  f32 euler_ypr_[3];
  f32 sample_freq_;
  volatile f32 integral_fb_x_ = 0.0f, integral_fb_y_ = 0.0f,
               integral_fb_z_ = 0.0f;  // integral error terms scaled by Ki
};

}  // namespace irobot_ec::modules::algorithm

#endif  // EC_LIB_MODULES_ALGORITHM_MAHONY_AHRS_H
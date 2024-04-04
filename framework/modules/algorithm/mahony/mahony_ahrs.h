
#ifndef EC_LIB_MODULES_ALGORITHM_MAHONY_AHRS_H
#define EC_LIB_MODULES_ALGORITHM_MAHONY_AHRS_H

#include <array>

#include "modules/typedefs.h"

namespace irobot_ec::modules::algorithm {

class MahonyAhrs {
 public:
  MahonyAhrs(fp32 sample_freq = 1000.0f, fp32 kp = 1.0f, fp32 ki = 0.0f);
  ~MahonyAhrs() = default;

  void Update(float gx, float gy, float gz, float ax, float ay, float az, float mx, float my, float mz);
  void UpdateImu(float gx, float gy, float gz, float ax, float ay, float az);
  [[nodiscard]] fp32 quat_w() const;
  [[nodiscard]] fp32 quat_x() const;
  [[nodiscard]] fp32 quat_y() const;
  [[nodiscard]] fp32 quat_z() const;
  [[nodiscard]] fp32 euler_yaw() const;
  [[nodiscard]] fp32 euler_pitch() const;
  [[nodiscard]] fp32 euler_roll() const;

 private:
  fp32 two_kp_, two_ki_;
  fp32 q0_, q1_, q2_, q3_;
  fp32 recip_norm_;
  fp32 q0q0_, q0q1_, q0q2_, q0q3_, q1q1_, q1q2_, q1q3_, q2q2_, q2q3_, q3q3_;
  fp32 hx_, hy_, bx_, bz_;
  fp32 halfvx_, halfvy_, halfvz_, halfwx_, halfwy_, halfwz_;
  fp32 halfex_, halfey_, halfez_;
  fp32 qa_, qb_, qc_;
  fp32 quaternion_[4];
  fp32 euler_ypr_[3];
  fp32 sample_freq_;
  volatile fp32 integral_fb_x_ = 0.0f, integral_fb_y_ = 0.0f,
                integral_fb_z_ = 0.0f;  // integral error terms scaled by Ki
};

}  // namespace irobot_ec::modules::algorithm

#endif  // EC_LIB_MODULES_ALGORITHM_MAHONY_AHRS_H
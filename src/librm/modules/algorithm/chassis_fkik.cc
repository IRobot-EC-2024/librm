/*
  Copyright (c) 2024 XDU-IRobot

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
*/

/**
 * @file  librm/modules/algorithm/chassis_fkik.cc
 * @brief 各种常见底盘构型的正逆运动学解算
 */

#include "chassis_fkik.h"

#include <cmath>
#include <initializer_list>

/**
 * @brief max函数，由于一些神秘原因gcc-arm-none-eabi没有实现这个函数？？？？？？？（问号脸
 */
template <typename Tp>
static constexpr Tp max_(std::initializer_list<Tp> il) {
  if (il.size() == 0) {
    return 0;
  }
  Tp max = *il.begin();
  for (auto &i : il) {
    if (i > max) {
      max = i;
    }
  }
  return max;
}

namespace rm::modules::algorithm {
/**
 * @param wheel_base    轮子间距
 * @param wheel_track   轮子轴距
 */
MecanumChassis::MecanumChassis(f32 wheel_base, f32 wheel_track)
    : wheel_base_(wheel_base), wheel_track_(wheel_track), speeds_{0} {}

/**
 * @param vx    x轴方向的速度
 * @param vy    y轴方向的速度
 * @param wz    z轴方向的角速度
 */
void MecanumChassis::Forward(f32 vx, f32 vy, f32 wz) {
  this->speeds_[0] = vx - vy - (this->wheel_base_ + this->wheel_track_) * wz;
  this->speeds_[1] = vx + vy + (this->wheel_base_ + this->wheel_track_) * wz;
  this->speeds_[2] = vx + vy - (this->wheel_base_ + this->wheel_track_) * wz;
  this->speeds_[3] = vx - vy + (this->wheel_base_ + this->wheel_track_) * wz;
}

/**
 * @param chassis_radius 底盘圆周半径
 */
SteeringChassis::SteeringChassis(f32 chassis_radius) : chassis_radius_(chassis_radius) {}

/**
 * @param vx    x轴方向的速度
 * @param vy    y轴方向的速度
 * @param wz    z轴方向的角速度
 */
void SteeringChassis::Forward(f32 vx, f32 vy, f32 wz) {
  front_wheel_speed_ =
      sqrtf(pow(vy - wz * chassis_radius_ * sqrtf(2) / 2, 2) + pow(vx - wz * chassis_radius_ * sqrtf(2) / 2, 2));
  back_wheel_speed_ =
      sqrtf(pow(vy + wz * chassis_radius_ * sqrtf(2) / 2, 2) + pow(vx - wz * chassis_radius_ * sqrtf(2) / 2, 2));
  left_wheel_speed_ =
      sqrtf(pow(vy + wz * chassis_radius_ * sqrtf(2) / 2, 2) + pow(vx + wz * chassis_radius_ * sqrtf(2) / 2, 2));
  right_wheel_speed_ =
      sqrtf(pow(vy - wz * chassis_radius_ * sqrtf(2) / 2, 2) + pow(vx + wz * chassis_radius_ * sqrtf(2) / 2, 2));

  front_steer_angle_ = atan2f(vy - wz * chassis_radius_ * sqrtf(2) / 2, vx - wz * chassis_radius_ * sqrtf(2) / 2);
  back_steer_angle_ = atan2f(vy + wz * chassis_radius_ * sqrtf(2) / 2, vx - wz * chassis_radius_ * sqrtf(2) / 2);
  left_steer_angle_ = atan2f(vy + wz * chassis_radius_ * sqrtf(2) / 2, vx + wz * chassis_radius_ * sqrtf(2) / 2);
  right_steer_angle_ = atan2f(vy - wz * chassis_radius_ * sqrtf(2) / 2, vx + wz * chassis_radius_ * sqrtf(2) / 2);

  f32 max_speed = max_({std::abs(front_wheel_speed_), std::abs(back_wheel_speed_), std::abs(left_wheel_speed_),
                        std::abs(right_wheel_speed_)});
  if (max_speed > 1) {
    front_wheel_speed_ /= max_speed;
    back_wheel_speed_ /= max_speed;
    left_wheel_speed_ /= max_speed;
    right_wheel_speed_ /= max_speed;
  }
}

/**
 * @param vx    x轴方向的速度
 * @param vy    y轴方向的速度
 * @param wz    z轴方向的角速度
 */
void QuadOmniChassis::Forward(float vx, float vy, float wz) {
  front_wheel_speed_ = vx + wz;
  back_wheel_speed_ = -vx + wz;
  left_wheel_speed_ = vy + wz;
  right_wheel_speed_ = -vy + wz;
  // normalize
  f32 max_speed = max_({std::abs(front_wheel_speed_), std::abs(back_wheel_speed_), std::abs(left_wheel_speed_),
                        std::abs(right_wheel_speed_)});
  if (max_speed > 1) {
    front_wheel_speed_ /= max_speed;
    back_wheel_speed_ /= max_speed;
    left_wheel_speed_ /= max_speed;
    right_wheel_speed_ /= max_speed;
  }
}

/**
 * @param front_wheel_speed    前轮速度
 * @param back_wheel_speed     后轮速度
 * @param left_wheel_speed     左轮速度
 * @param right_wheel_speed    右轮速度
 */
void QuadOmniChassis::Inverse(float front_wheel_speed, float back_wheel_speed, float left_wheel_speed,
                              float right_wheel_speed) {
  vx_ = (front_wheel_speed + back_wheel_speed - left_wheel_speed - right_wheel_speed) / 4;
  vy_ = (front_wheel_speed - back_wheel_speed + left_wheel_speed - right_wheel_speed) / 4;
  wz_ = (front_wheel_speed - back_wheel_speed - left_wheel_speed + right_wheel_speed) / 4;
  // normalize
  f32 max_speed = max_({std::abs(vx_), std::abs(vy_), std::abs(wz_)});
  if (max_speed > 1) {
    vx_ /= max_speed;
    vy_ /= max_speed;
    wz_ /= max_speed;
  }
}

}  // namespace rm::modules::algorithm
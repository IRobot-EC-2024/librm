/**
 * @file  modules/algorithm/mecanum/mecanum.cc
 * @brief 麦轮正运动学解算
 */

#include "mecanum.h"

using namespace irobot_ec::modules::algorithm;

/**
 * @param wheel_base    轮子间距
 * @param wheel_track   轮子轴距
 */
Mecanum::Mecanum(f32 wheel_base, f32 wheel_track) : wheel_base_(wheel_base), wheel_track_(wheel_track), speeds_{0} {}

/**
 * @brief 计算麦轮底盘的四个轮子的速度
 * @param vx    x轴方向的速度
 * @param vy    y轴方向的速度
 * @param wz    z轴方向的角速度
 */
void Mecanum::Calculate(f32 vx, f32 vy, f32 wz) {
  this->speeds_[0] = vx - vy - (this->wheel_base_ + this->wheel_track_) * wz;
  this->speeds_[1] = vx + vy + (this->wheel_base_ + this->wheel_track_) * wz;
  this->speeds_[2] = vx + vy - (this->wheel_base_ + this->wheel_track_) * wz;
  this->speeds_[3] = vx - vy + (this->wheel_base_ + this->wheel_track_) * wz;
}

/**
 * @return  左前轮速度
 */
[[nodiscard]] f32 Mecanum::v_lf() const { return this->speeds_[0]; }

/**
 * @return  右前轮速度
 */
[[nodiscard]] f32 Mecanum::v_rf() const { return this->speeds_[1]; }

/**
 * @return  左后轮速度
 */
[[nodiscard]] f32 Mecanum::v_lb() const { return this->speeds_[2]; }

/**
 * @return  右后轮速度
 */
[[nodiscard]] f32 Mecanum::v_rb() const { return this->speeds_[3]; }

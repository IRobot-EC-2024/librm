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
 * @file  modules/algorithm/mecanum.h
 * @brief 麦轮正运动学解算
 */

#ifndef IROBOTEC_MODULES_ALGORITHM_MECANUM_MECANUM_H
#define IROBOTEC_MODULES_ALGORITHM_MECANUM_MECANUM_H

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

#endif  // IROBOTEC_MODULES_ALGORITHM_MECANUM_MECANUM_H

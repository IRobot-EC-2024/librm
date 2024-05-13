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
 * @file  modules/algorithm/pid/pid.cc
 * @brief PID控制器
 */

#include "pid.h"

#include <cstring>

#include "utils.hpp"

namespace irobot_ec::modules::algorithm {

using irobot_ec::modules::algorithm::utils::absConstrain;

PID::PID(PIDType type, f32 kp, f32 ki, f32 kd, f32 max_out, f32 max_iout)
    : kp_(kp),
      ki_(ki),
      kd_(kd),
      type_(type),
      max_out_(max_out),
      max_iout_(max_iout),
      use_external_diff_input_(false),
      external_diff_input_(nullptr) {
#if defined(ARM_MATH_DSP)
  if (type == PIDType::kDsp) {
    this->dsp_pid_ = {
        .Kp = kp,
        .Ki = ki,
        .Kd = kd,
    };
  }
  arm_pid_init_f32(&this->dsp_pid_, 1);
#endif
}

PID::PID(PIDType type, f32 kp, f32 ki, f32 kd, f32 max_out, f32 max_iout, f32 *external_diff_input)
    : kp_(kp),
      ki_(ki),
      kd_(kd),
      type_(type),
      max_out_(max_out),
      max_iout_(max_iout),
      use_external_diff_input_(true),
      external_diff_input_(external_diff_input) {
#if defined(ARM_MATH_DSP)
  if (type == PIDType::kDsp) {
    this->dsp_pid_ = {
        .Kp = kp,
        .Ki = ki,
        .Kd = kd,
    };
  }
  arm_pid_init_f32(&this->dsp_pid_, 1);
#endif
}

void PID::update(f32 set, f32 ref) {
  this->error_[2] = this->error_[1];
  this->error_[1] = this->error_[0];

  this->set_ = set;
  this->fdb_ = ref;
  this->error_[0] = set - ref;

  switch (this->type_) {
    case PIDType::kPosition:
      this->p_out_ = this->kp_ * this->error_[0];
      this->i_out_ += this->ki_ * this->error_[0];

      // update derivative term
      this->d_buf_[2] = this->d_buf_[1];
      this->d_buf_[1] = this->d_buf_[0];
      this->d_buf_[0] = use_external_diff_input_ ? *(this->external_diff_input_) : (this->error_[0] - this->error_[1]);

      this->d_out_ = this->kd_ * this->d_buf_[0];
      this->i_out_ = absConstrain(this->i_out_, this->max_iout_);
      this->out_ = this->p_out_ + this->i_out_ + this->d_out_;
      this->out_ = absConstrain(this->out_, this->max_out_);
      break;

    case PIDType::kDelta:
      this->p_out_ = this->kp_ * (this->error_[0] - this->error_[1]);
      this->i_out_ = this->ki_ * this->error_[0];

      this->d_buf_[2] = this->d_buf_[1];
      this->d_buf_[1] = this->d_buf_[0];
      this->d_buf_[0] = use_external_diff_input_ ? *(this->external_diff_input_)
                                                 : (this->error_[0] - 2.0f * this->error_[1] + this->error_[2]);

      this->d_out_ = this->kd_ * this->d_buf_[0];
      this->out_ += this->p_out_ + this->i_out_ + this->d_out_;
      this->out_ = absConstrain(this->out_, this->max_out_);
      break;

#if defined(ARM_MATH_DSP)
    case PIDType::kDsp:
      this->out_ = arm_pid_f32(&this->dsp_pid_, this->error_[0]);
      this->out_ = absConstrain(this->out_, this->max_out_);
      break;
#endif
  }
}

void PID::clear() {
  this->set_ = 0;
  this->fdb_ = 0;
  this->out_ = 0;
  this->p_out_ = 0;
  this->i_out_ = 0;
  this->d_out_ = 0;
  memset(this->d_buf_, 0, 3);
  memset(this->error_, 0, 3);
#if defined(ARM_MATH_DSP)
  if (this->type_ == PIDType::kDsp) {
    arm_pid_reset_f32(&this->dsp_pid_);
  }
#endif
}

void PID::switchParameter(f32 kp, f32 ki, f32 kd, f32 max_out, f32 max_iout) {
  this->kp_ = kp;
  this->ki_ = ki;
  this->kd_ = kd;
  this->max_out_ = max_out;
  this->max_iout_ = max_iout;
#if defined(ARM_MATH_DSP)
  if (this->type_ == PIDType::kDsp) {
    this->dsp_pid_ = {
        .Kp = kp,
        .Ki = ki,
        .Kd = kd,
    };
    arm_pid_init_f32(&this->dsp_pid_, 1);
  }
#endif
}

f32 PID::value() const { return this->out_; }

RingPID::RingPID(PIDType type, f32 kp, f32 ki, f32 kd, f32 max_out, f32 max_iout, f32 cycle)
    : PID(type, kp, ki, kd, max_out, max_iout), cycle_(cycle) {}

void RingPID::update(f32 set, f32 ref) {
  this->error_[2] = this->error_[1];
  this->error_[1] = this->error_[0];

  this->set_ = set;
  this->fdb_ = ref;
  this->error_[0] = set - ref;

  this->handleZeroCrossing();

  switch (this->type_) {
    case PIDType::kPosition:
      this->p_out_ = this->kp_ * this->error_[0];
      this->i_out_ += this->ki_ * this->error_[0];

      // update derivative term
      this->d_buf_[2] = this->d_buf_[1];
      this->d_buf_[1] = this->d_buf_[0];
      this->d_buf_[0] = use_external_diff_input_ ? *(this->external_diff_input_) : (this->error_[0] - this->error_[1]);

      this->d_out_ = this->kd_ * this->d_buf_[0];
      this->i_out_ = absConstrain(this->i_out_, this->max_iout_);
      this->out_ = this->p_out_ + this->i_out_ + this->d_out_;
      this->out_ = absConstrain(this->out_, this->max_out_);
      break;

    case PIDType::kDelta:
      this->p_out_ = this->kp_ * (this->error_[0] - this->error_[1]);
      this->i_out_ = this->ki_ * this->error_[0];

      this->d_buf_[2] = this->d_buf_[1];
      this->d_buf_[1] = this->d_buf_[0];
      this->d_buf_[0] = use_external_diff_input_ ? *(this->external_diff_input_)
                                                 : (this->error_[0] - 2.0f * this->error_[1] + this->error_[2]);

      this->d_out_ = this->kd_ * this->d_buf_[0];
      this->out_ += this->p_out_ + this->i_out_ + this->d_out_;
      this->out_ = absConstrain(this->out_, this->max_out_);
      break;

#if defined(ARM_MATH_DSP)
    case PIDType::kDsp:
      this->out_ = arm_pid_f32(&this->dsp_pid_, this->error_[0]);
      this->out_ = absConstrain(this->out_, this->max_out_);
      break;
#endif
  }
}

void RingPID::handleZeroCrossing() {
  if (this->error_[0] > (this->cycle_ / 2)) {
    this->error_[0] -= this->cycle_;
  } else if (this->error_[0] < (-this->cycle_ / 2)) {
    this->error_[0] += this->cycle_;
  }
}

}  // namespace irobot_ec::modules::algorithm

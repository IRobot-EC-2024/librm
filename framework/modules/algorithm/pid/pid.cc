
/**
 * @file  modules/pid/c_pid.cpp
 * @brief PID控制器
 */

#include "pid.h"

#include <cstring>

#include "modules/algorithm/utils/utils.hpp"

using namespace irobot_ec::modules::algorithm::PID;
using irobot_ec::modules::algorithm::utils::absConstrain;

PID::PID(PIDType type, fp32 kp, fp32 ki, fp32 kd, fp32 max_out, fp32 max_iout)
    : kp_(kp), ki_(ki), kd_(kd), type_(type), max_out_(max_out), max_iout_(max_iout), use_external_diff_input_(false) {}

PID::PID(PIDType type, fp32 kp, fp32 ki, fp32 kd, fp32 max_out, fp32 max_iout, fp32 *external_diff_input)
    : kp_(kp),
      ki_(ki),
      kd_(kd),
      type_(type),
      max_out_(max_out),
      max_iout_(max_iout),
      use_external_diff_input_(true),
      external_diff_input_(external_diff_input) {}

void PID::update(fp32 set, fp32 ref) {
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
}

void PID::switchParameter(fp32 kp, fp32 ki, fp32 kd, fp32 max_out, fp32 max_iout) {
  this->kp_ = kp;
  this->ki_ = ki;
  this->kd_ = kd;
  this->max_out_ = max_out;
  this->max_iout_ = max_iout;
}

fp32 PID::value() const { return this->out_; }

RingPID::RingPID(PIDType type, fp32 kp, fp32 ki, fp32 kd, fp32 max_out, fp32 max_iout, fp32 cycle)
    : PID(type, kp, ki, kd, max_out, max_iout), cycle_(cycle) {}

void RingPID::update(fp32 set, fp32 ref) {
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
  }
}

void RingPID::handleZeroCrossing() {
  if (this->error_[0] > (this->cycle_ / 2)) {
    this->error_[0] -= this->cycle_;
  } else if (this->error_[0] < (-this->cycle_ / 2)) {
    this->error_[0] += this->cycle_;
  }
}

/* EOF */

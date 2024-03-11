
/***
 * @file  modules/pid/c_pid.h
 * @brief PID controller declaration
 */

#pragma once

#ifndef EC_LIB_MODULES_ALGORITHM_PID_H
#define EC_LIB_MODULES_ALGORITHM_PID_H

#include <memory>

#include "modules/typedefs.h"

namespace modules::algorithm::PID {

enum class PIDType {
  kPosition,
  kDelta,
};

/***
 * @brief general PID controller
 */
class PID {
 public:
  PID() = delete;
  PID(PIDType type, fp32 kp, fp32 ki, fp32 kd, fp32 max_out, fp32 max_iout);
  PID(PIDType type, fp32 kp, fp32 ki, fp32 kd, fp32 max_out, fp32 max_iout,
      fp32 *external_diff_input);
  virtual void update(fp32 set, fp32 ref);
  void clear();
  void switchParameter(fp32 kp, fp32 ki, fp32 kd, fp32 max_out, fp32 max_iout);
  [[nodiscard]] fp32 value() const;

 protected:
  fp32 kp_;
  fp32 ki_;
  fp32 kd_;

  fp32 max_out_{};
  fp32 max_iout_{};

  fp32 set_{};
  fp32 fdb_{};

  fp32 out_{};
  fp32 p_out_{};
  fp32 i_out_{};
  fp32 d_out_{};
  fp32 d_buf_[3]{};  // 0: latest, 1: last, 2: last last
  fp32 error_[3]{};  // 0: latest, 1: last, 2: last last

  fp32 *external_diff_input_;
  bool use_external_diff_input_;

  PIDType type_;

};  // class PID

/***
 * @brief PID controller for periodic target, for example position controlling
 * of a motor
 */
class RingPID : public PID {
 public:
  RingPID() = delete;
  RingPID(PIDType type, fp32 kp, fp32 ki, fp32 kd, fp32 max_out, fp32 max_iout,
          fp32 cycle);
  void update(fp32 set, fp32 ref) override;

 protected:
  void handleZeroCrossing();

  fp32 cycle_;

};  // class RingPID

}  // namespace modules::algorithm::PID

#endif  // EC_LIB_MODULES_ALGORITHM_PID_H

/* EOF */

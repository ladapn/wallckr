#ifndef STEERING_SERVO_H
#define STEERING_SERVO_H

#include <zephyr/drivers/pwm.h>

class SteeringServo {
  const struct pwm_dt_spec steering_servo =
      PWM_DT_SPEC_GET(DT_NODELABEL(servo));
  const uint32_t min_pulse_us = DT_PROP(DT_NODELABEL(servo), min_pulse);
  const uint32_t max_pulse_us = DT_PROP(DT_NODELABEL(servo), max_pulse);
  /** Minimal servo angle the steering can handle */
  const uint16_t min_steering_angle_deg = 20;
  /** Maximal servo angle the steering can handle */
  const uint16_t max_steering_angle_deg = 160;
  /** Minimal angle the servo can when not connected to the steering */
  const uint16_t min_angle_deg = 0;
  /** Maximal angle the servo can when not connected to the steering */
  const uint16_t max_angle_deg = 180;

public:
  /**
   * Set steering servo angle
   * @param[in] angle desired angle in degrees
   * @return zero if PWM duty cycle controlling the servo was changed
   * successfully, a negative value otherwise: -EINVAL is returned if angle is
   * outside of <min_steering_angle_deg; max_steering_angle_deg> interval and if
   * pwm_set_pulse_dt fails, its return value is returned
   */
  int set_angle(int angle);
  /**
   * Checks if the servo is ready
   * @return true if the servo is ready, false otherwise
   */
  bool is_ready();
};

#endif // STEERING_SERVO_H
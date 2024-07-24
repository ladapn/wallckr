#include "SteeringServo.h"
#include <zephyr/logging/log.h>

LOG_MODULE_DECLARE(Motion, LOG_LEVEL_INF);

// TODO: to be properly tested + mention limitations
int map(int x, int in_min, int in_max, int out_min, int out_max)
{
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

int SteeringServo::set_angle(int angle)
{
	// TODO make these kconfigs
	if (angle > max_steering_angle_deg || angle < min_steering_angle_deg) {
		LOG_WRN("The specified steering servo angle of %d is beyond the "
			"permissible range of <%d, %d>",
			angle, min_steering_angle_deg, max_steering_angle_deg);
		return -EINVAL;
	}

	uint32_t pulse_width_us = map(angle, min_angle_deg, max_angle_deg, min_pulse_us, max_pulse_us);
	int ret = pwm_set_pulse_dt(&steering_servo, PWM_USEC(pulse_width_us));
	if (ret < 0) {
		LOG_ERR("Unable to adjust the steering servo pulse width to %d with error "
			"code: %d",
			pulse_width_us, ret);
		return ret;
	}

	return 0;
}

bool SteeringServo::is_ready()
{
	if (!pwm_is_ready_dt(&steering_servo)) {
		LOG_ERR("Steering servo %s is not ready", steering_servo.dev->name);
		return false;
	}

	return true;
}
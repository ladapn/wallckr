#include "Motion.h"
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(Motion, LOG_LEVEL_INF);

bool Motion::is_ready()
{
	return steering_servo.is_ready(); // TODO: && motor.is_ready();
}

bool Motion::set_speed(int speed)
{
	if (disabled) {
		return false;
	}

	if (prev_speed == speed) {
		return false;
	}

	prev_speed = speed;
	return motor.set_speed(speed);
}

bool Motion::set_steering_angle(int angle)
{
	if (disabled) {
		return false;
	}

	if (oldServo != angle) {
		oldServo = angle;
		steering_servo.set_angle(angle);
	} else {
		return false;
	}

	return true;
}

bool Motion::set_speed_and_angle(int speed, int angle)
{
	bool retval = set_speed(speed);
	bool retval2 = set_steering_angle(angle);

	return retval && retval2;
}

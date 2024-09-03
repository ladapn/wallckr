#include "Motion.h"
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(Motion, CONFIG_MOTION_LOG_LEVEL);

bool Motion::is_ready()
{
	return steering_servo.is_ready() && motor.is_ready();
}

int Motion::set_speed(int speed)
{
	if (disabled) {
		LOG_INF("Unable to adjust speed, Motion module deactivated");
		return -EBUSY;
	}

	if (prev_speed == speed) {
		LOG_DBG("Tried to adjust speed to a value that is already set, no change made");
		return 0;
	}

	prev_speed = speed;
	return motor.set_speed(speed);
}

int Motion::set_steering_angle(int angle)
{
	if (disabled) {
        LOG_INF("Unable to adjust steering angle, Motion module deactivated");
		return -EBUSY;
	}

    if (oldServo == angle) {
        LOG_DBG("Tried to adjust steering angle to a value that is already set, no change made");
        return 0;
    }

	oldServo = angle;
	return steering_servo.set_angle(angle);
}

bool Motion::set_speed_and_angle(int speed, int angle)
{
	bool retval = set_speed(speed);
	bool retval2 = set_steering_angle(angle);

	return retval && retval2;
}

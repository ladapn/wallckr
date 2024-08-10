#include "Motor.h"
#include "MotionConstants.h"
#include <zephyr/logging/log.h>

/**
 * Alias for pin states causing forward/backward motor run
 */
//enum class MotorDirection { FORWARD = HIGH, BACKWARD = LOW };
enum class MotorDirection { FORWARD = 1, BACKWARD = 0 };

LOG_MODULE_DECLARE(Motion, LOG_LEVEL_INF);

Motor::Motor(int dir_pin, int pwm_control_pin, int brake_control_pin)
{
    /*direction_pin = dir_pin;
    pwm_pin = pwm_control_pin;
    brake_pin = brake_control_pin;*/
}
    
int Motor::set_speed(int speed)
{
    if ((speed > MAX_SPEED) || (speed < -MAX_SPEED))
    {
        LOG_WRN("The specified motor speed of %d is beyond the "
			"permissible range of <%d, %d>",
			speed, -MAX_SPEED, MAX_SPEED);
		return -EINVAL;
    }

    auto motorDirection = MotorDirection::FORWARD;

    if (speed < 0)
    {
        motorDirection = MotorDirection::BACKWARD;
        //speed = abs(speed);
    }

    //digitalWrite(direction_pin, static_cast<uint8_t>(motorDirection)); // Set motor direction
    //analogWrite(pwm_pin, speed);                                       // Set the speed of the motor

    return true;
}

void Motor::stop()
{
    //analogWrite(pwm_pin, 0);
}

bool Motor::is_ready()
{
    // TODO
    return true;
}

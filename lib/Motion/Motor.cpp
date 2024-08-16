#include "Motor.h"
#include "MotionConstants.h"
#include <zephyr/logging/log.h>
#include <zephyr/drivers/gpio.h>

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

int Motor::initialize()
{
    if (!is_ready()) {
        LOG_ERR("Cannot initialize motor, because it is not ready");
        return -EBUSY; 
    }

    int err = gpio_pin_configure_dt(&enable, GPIO_OUTPUT_INACTIVE);
	if (err != 0) {
		LOG_ERR("Configuring motor's 'enable' GPIO pin failed: %d", err);
		return err;
	}

    err = gpio_pin_configure_dt(&in1, GPIO_OUTPUT_INACTIVE);
	if (err != 0) {
		LOG_ERR("Configuring motor's 'in1' GPIO pin failed: %d", err);
		return err;
	}

    err = gpio_pin_configure_dt(&in2, GPIO_OUTPUT_INACTIVE);
	if (err != 0) {
		LOG_ERR("Configuring motor's 'in2' GPIO pin failed: %d", err);
		return err;
	}

    return 0;
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
    if (!pwm_is_ready_dt(&pwm)) {
		LOG_ERR("Motor's PWM output is not ready");
		return false;
	}

    if (!gpio_is_ready_dt(&enable)) {
		LOG_ERR("Motor's 'enable' pin not ready");
		return false;
	}

    if (!gpio_is_ready_dt(&in1)) {
		LOG_ERR("Motor's 'in1' pin not ready");
		return false;
	}

    if (!gpio_is_ready_dt(&in2)) {
		LOG_ERR("Motor's 'in2' pin not ready");
		return false;
	}

    return true;
}

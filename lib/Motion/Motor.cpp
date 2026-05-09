#include "Motor.h"
#include <zephyr/logging/log.h>
#include <zephyr/drivers/gpio.h>
#include <stdlib.h>

// TODO: vlastni log module pro Motor
// TODO: prejmenovat motor na motor_controller nebo neco podobneho, aby bylo jasne, ze to neni jenom wrapper pro PWM, ale i pro GPIO a logiku kolem toho
LOG_MODULE_DECLARE(Motion, CONFIG_MOTION_LOG_LEVEL);

int Motor::initialize()
{
    if (!is_ready()) {
        LOG_ERR("Cannot initialize motor, because it is not ready");
        return -EBUSY; 
    }

    int err = gpio_pin_configure_dt(&ph, GPIO_OUTPUT_INACTIVE);
	if (err != 0) {
		LOG_ERR("Configuring motor's 'PH' GPIO pin failed: %d", err);
		return err;
	}

    return 0;
}

int Motor::set_direction(MotorDirection dir) 
{
    int ph_state = (dir == MotorDirection::FORWARD) ? 1 : 0;

    int err = gpio_pin_set_dt(&ph, ph_state);
    if (err != 0) {
        LOG_ERR("Cannot set motor direction, setting ph GPIO pin level failed: %d", err);
        return err;
    }

    return 0;
}

int Motor::set_pwm(uint32_t pulse_width_ns) 
{
    LOG_DBG("Setting the motor PWM to %d ns", pulse_width_ns);
    int err = pwm_set_pulse_dt(&pwm, PWM_NSEC(pulse_width_ns));
	if (err < 0) {
		LOG_ERR("Unable to adjust motor PWM pulse width to %d ns with error code: %d",
			pulse_width_ns, err);
		return err;
	}

    return 0;
}
    
int Motor::set_speed(int speed)
{
    if ((speed >= max_speed) || (speed <= min_speed))
    {
        LOG_WRN("The specified motor speed of %d is beyond the "
			"permissible range of <%d, %d>",
			speed, min_speed, max_speed);
		return -EINVAL;
    }

    if (speed == 0) {
        return stop();
    }

    auto motor_direction = MotorDirection::FORWARD;

    if(speed < 0) {
        motor_direction = MotorDirection::BACKWARD;
        speed = abs(speed);
    }

    int err = set_direction(motor_direction);
    if(err) {
        LOG_ERR("Unable to set motor direction - error code: %d", err);
        return err;
    }

    uint32_t pulse_width_ns = (pwm.period * speed) / 100;
    LOG_DBG("Setting the motor speed to %d %%", speed);
    err = set_pwm(pulse_width_ns);
	if (err < 0) {
		LOG_ERR("Unable to adjust the motor speed to %d %% with error code: %d",
			speed, err);
		return err;
	}

    return 0;
}

int Motor::stop()
{
    return set_pwm(0);
}

bool Motor::is_ready()
{
    if (!pwm_is_ready_dt(&pwm)) {
		LOG_ERR("Motor's PWM output is not ready");
		return false;
	}

    if (!gpio_is_ready_dt(&ph)) {
		LOG_ERR("Motor's Phase pin not ready");
		return false;
	}

    return true;
}

#ifndef MOTOR_H
#define MOTOR_H

#include <zephyr/drivers/pwm.h>
#include <zephyr/drivers/gpio.h>

// TODO: pins should be inputs to the constructor + references & forward declaration 

class Motor {
  int direction_pin;
  int brake_pin;

  const struct pwm_dt_spec pwm =
    PWM_DT_SPEC_GET(DT_NODELABEL(motor));

  const struct gpio_dt_spec enable = GPIO_DT_SPEC_GET(DT_NODELABEL(motor), en_gpios);
  const struct gpio_dt_spec in1 = GPIO_DT_SPEC_GET(DT_NODELABEL(motor), in1_gpios);
  const struct gpio_dt_spec in2 = GPIO_DT_SPEC_GET(DT_NODELABEL(motor), in2_gpios);

public:
  Motor(int dir_pin, int pwm_control_pin, int break_control_pin);
  Motor();
  int set_speed(int speed);
  void stop();
  /**
   * Checks if the motor is ready
   * @return true if the motor is ready, false otherwise
   */
  bool is_ready();
};

#endif // MOTOR_H
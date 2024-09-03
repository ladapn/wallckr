#ifndef MOTOR_H
#define MOTOR_H

#include <zephyr/drivers/pwm.h>
#include <zephyr/drivers/gpio.h>

// TODO: pins should be inputs to the constructor + references & forward declaration 

class Motor {

  enum class MotorDirection { FORWARD = 1, BACKWARD = 0 };
  const struct pwm_dt_spec pwm =
    PWM_DT_SPEC_GET(DT_NODELABEL(motor));

  const struct gpio_dt_spec enable = GPIO_DT_SPEC_GET(DT_NODELABEL(motor), en_gpios);
  const struct gpio_dt_spec in1 = GPIO_DT_SPEC_GET(DT_NODELABEL(motor), in1_gpios);
  const struct gpio_dt_spec in2 = GPIO_DT_SPEC_GET(DT_NODELABEL(motor), in2_gpios);

  // TODO take these from DT
  const int max_speed = 100;
  const int min_speed = -100;

  int set_direction(MotorDirection dir);
  int set_pwm(uint32_t pulse_width_ns);

public:
  Motor() {};
  int set_speed(int speed);
  int stop();
  /**
   * Checks if the motor is ready
   * @return true if the motor is ready, false otherwise
   */
  bool is_ready();
  int initialize();
};

#endif // MOTOR_H
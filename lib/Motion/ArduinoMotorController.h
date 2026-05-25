#ifndef ARDUINO_MOTOR_CONTROLLER_H
#define ARDUINO_MOTOR_CONTROLLER_H

#include "IMotorController.h"

class ArduinoMotorController : public IMotorController {
  int pwm_pin;
  int direction_pin;
  int brake_pin;

public:
  ArduinoMotorController(int dir_pin, int pwm_control_pin,
                         int break_control_pin);
  bool set_speed(int speed) override;
  void stop() override;
  int initialize() override { return 0; }
  bool is_ready() override { return true; }
};

#endif // ARDUINO_MOTOR_CONTROLLER_H

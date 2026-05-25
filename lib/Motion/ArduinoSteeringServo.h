#ifndef STEERING_SERVO_H
#define STEERING_SERVO_H

#include "ISteeringServo.h"
#include <Servo.h>

class ArduinoSteeringServo : public ISteeringServo {
  Servo steering_servo;
  int servo_pin;

public:
  explicit ArduinoSteeringServo(int pin) : servo_pin(pin) {}
  bool set_angle(int angle) override;
  int initialize() override;
  bool is_ready() override { return true; }
};

#endif // STEERING_SERVO_H

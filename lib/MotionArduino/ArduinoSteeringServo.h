#ifndef STEERING_SERVO_H
#define STEERING_SERVO_H

#include "ISteeringServo.h"
#include <Servo.h>

class ArduinoSteeringServo : public ISteeringServo {
  Servo steering_servo;

public:
  explicit ArduinoSteeringServo(int pin);
  bool set_angle(int angle) override;
};

#endif // STEERING_SERVO_H

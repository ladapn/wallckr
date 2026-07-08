#include "ArduinoSteeringServo.h"

ArduinoSteeringServo::ArduinoSteeringServo(int pin) {
  steering_servo.attach(pin);
}

bool ArduinoSteeringServo::set_angle(int angle) {
  steering_servo.write(angle);
  return true;
}

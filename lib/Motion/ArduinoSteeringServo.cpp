#include "ArduinoSteeringServo.h"

bool ArduinoSteeringServo::set_angle(int angle) {
  steering_servo.write(angle);
  return true;
}

int ArduinoSteeringServo::initialize() {

  if (steering_servo.attach(servo_pin) == INVALID_SERVO) {
    return -1;
  }

  return 0;
}

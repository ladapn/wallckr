#include "SteeringServo.h"

void SteeringServo::init(int pin)
{
    steering_servo.attach(pin);
}

void SteeringServo::set_angle(int angle)
{
    steering_servo.write(angle);
}
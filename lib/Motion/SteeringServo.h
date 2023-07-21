#ifndef STEERING_SERVO_H
#define STEERING_SERVO_H

#include <Servo.h>

class SteeringServo
{
    Servo steering_servo;
public:
    void init(int pin);
    void set_angle(int angle);

};

#endif  //STEERING_SERVO_H
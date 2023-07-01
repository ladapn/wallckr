#include "Motion.h"

bool Motion::setSpeed(int speed)
{
    if (disabled)
    {
        return false;
    }
    
    if (prev_speed == speed)
    {
        return false;
    }

    if ((speed > MAX_SPEED) && (speed < -MAX_SPEED))
    {
        return false;
    }
    
    prev_speed = speed;
    auto motorDirection = MotorDirection::FORWARD; // HIGH -> forward

    if (speed < 0)
    {
        motorDirection = MotorDirection::BACKWARD;
        speed = abs(speed);
    }

    digitalWrite(DIR_A, static_cast<uint8_t>(motorDirection)); // Set motor direction
    analogWrite(PWM_A, speed);             // Set the speed of the motor, 255 is the maximum value

    return true;
}

bool Motion::setAngle(int angle)
{
    if (disabled)
    {
        return false;
    }
    
    if (oldServo != angle)
    {
        oldServo = angle;
        steering_servo.write(angle);
    }
    else
    {
        return false; 
    }

    return true; 
}

bool Motion::set_speed_and_angle(int speed, int angle)
{
    bool retval = setSpeed(speed);
    bool retval2 = setAngle(angle);

    return retval && retval2; 
}

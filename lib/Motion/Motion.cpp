#include "Motion.h"

bool Motion::setSpeed(int speed)
{
    bool retval = false;

    if (!disabled)
    {
      if (prev_speed != speed)
      {
          prev_speed = speed;

          auto motorDirection = MotorDirection::FORWARD; // HIGH -> forward

          if ((speed <= MAX_SPEED) && (speed >= -MAX_SPEED))
          {
              if (speed < 0)
              {
                  motorDirection = MotorDirection::BACKWARD;
                  speed = abs(speed);
              }

              digitalWrite(DIR_A, static_cast<uint8_t>(motorDirection)); // Set motor direction
              analogWrite(PWM_A, speed);             // Set the speed of the motor, 255 is the maximum value

              retval = true;
          }
      }
    }

    return retval;
}

bool Motion::setAngle(int angle)
{
    bool retval = false;

    if (!disabled)
    {
      if (oldServo != angle)
      {
          oldServo = angle;
          steering_servo.write(angle);

          retval = true;
      }
    }

    return retval; 
}

bool Motion::set_speed_and_angle(int speed, int angle)
{
    bool retval = setSpeed(speed);
    bool retval2 = setAngle(angle);

    return retval && retval2; 
}

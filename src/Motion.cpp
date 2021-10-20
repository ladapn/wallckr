#include "Motion.h"

bool Motion::setSpeed(int spd)
{
    bool retval = false;

    if (!m_disabled)
    {
      if (m_prev_speed != spd)
      {
          m_prev_speed = spd;

          bool motorDirection = HIGH; // HIGH -> forward

          if ((spd <= MAX_SPD) && (spd >= -MAX_SPD))
          {
              if (spd < 0)
              {
                  motorDirection = LOW;
                  spd = abs(spd);
              }

              digitalWrite(DIR_A, motorDirection); // Set motor direction
              analogWrite(PWM_A, spd);             // Set the speed of the motor, 255 is the maximum value

              retval = true;
          }
      }
    }

    return retval;
}

bool Motion::setAngle(int angle)
{
    bool retval = false;

    if (!m_disabled)
    {
      if (m_oldServo != angle)
      {
          m_oldServo = angle;
          m_steering_servo.write(angle);

          retval = true;
      }
    }

    return retval; 
}

bool Motion::set_speed_and_angle(int spd, int angle)
{
    bool retval = setSpeed(spd);
    bool retval2 = setAngle(angle);

    return retval && retval2; 
}

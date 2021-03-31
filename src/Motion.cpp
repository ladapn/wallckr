#include "Motion.h"

bool Motion::setSpeed(int spd)
{
    bool retval = false;

    if (m_prev_speed != spd)
    {
        m_prev_speed = spd;

        bool motorDirection = HIGH; // HIGH -> forward

        if ((spd <= 255) && (spd >= -255))
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

    return retval;
}

bool Motion::setAngle(int angle)
{
    bool retval = false;

    if (m_oldServo != angle)
    {
        m_oldServo = angle;
        m_steering_servo.write(angle);

        retval = true;
    }

    return retval; 
}

bool Motion::set_speed_and_angle(int spd, int angle)
{
    bool retval = setSpeed(spd);
    bool retval2 = setAngle(angle);

    return retval && retval2; 
}

bool Motion::command(command_t incommingByte, int &desiredSPD, int &desiredServo)
{   
    if(incommingByte != NO_COMMAND)
    {
      switch(incommingByte)
      {
        case UP:
          if(desiredSPD < MAX_SPD)
          {
            desiredSPD += SPD_INCREMENT;

          }
        break;
        case DOWN:
          if(desiredSPD > -MAX_SPD)
          {
            desiredSPD -= SPD_INCREMENT;
          }
        break;
        case RIGHT:
          if(desiredServo > SERVO_MIN_RIGHT)
          {
            desiredServo -= SERVO_INCREMENT;
          }
        break;
        case LEFT:
          if(desiredServo < SERVO_MAX_LEFT)
          {
            desiredServo += SERVO_INCREMENT;
          }
        break;
        case UP_TRIANGLE:
          desiredServo = SERVO_CENTER;
        break;
        default:
          desiredSPD = 0; 
      }
    }

    return true; //FIXME
}
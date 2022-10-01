#include "BLEJoystickDecoder.h"
#include "MotionConstants.h"
#include <Arduino.h>

RobotCommand BLEJoystickDecoder::input_to_command(char in)
{
  RobotCommand command = RobotCommand::NO_COMMAND;

  switch(in)
  {
    case 'a':
      command = RobotCommand::UP;
    break;
    case 'b':
      command = RobotCommand::RIGHT;
    break;
    case 'c':
      command = RobotCommand::DOWN;
    break;
    case 'd':
      command = RobotCommand::LEFT;
    break;
    case 'e':
      command = RobotCommand::UP_TRIANGLE;
    break;
    case 'f':
      command = RobotCommand::RIGHT_CIRCLE;
    break;
    case 'g':
      command = RobotCommand::DOWN_X;
    break;
    case 'h':
      command = RobotCommand::LEFT_SQUARE;
    break;

  }

  return command; 
}

// decoding commands from BLE joystick Iphone app which sends commands in
// format "Aa\0" -> Upper case, lower case, zero for each button press ->
// buttons are labeled from A to H
RobotCommand BLEJoystickDecoder::BLE_joystick_decoder()
{
  RobotCommand command = RobotCommand::NO_COMMAND; 
  char current, prev;
  bool waiting4zero = false;
  const int DIFF_LOWER_UPPER = 'a' - 'A';

  while(!m_cmdSerial.available())
  {
    ;
  }

  prev = m_cmdSerial.read();

  while(command == RobotCommand::NO_COMMAND) 
  {
    while(!m_cmdSerial.available())
    {
      ;
    }
    
    current = m_cmdSerial.read();

    if(waiting4zero)
    {

      
      if(current == 0)
      {
        command = input_to_command(prev);
      }
      else
      {
        waiting4zero = false;
      }
      
    }
    else if(current - prev == DIFF_LOWER_UPPER) 
    {
      waiting4zero = true;
    }

    prev = current; 
  }

  return command; 
  
}

RobotCommand BLEJoystickDecoder::ovladacka_decoder()
{
  RobotCommand command = RobotCommand::NO_COMMAND; 
  char current;
  const int DIFF_LOWER_UPPER = 'a' - 'A';

  if(m_cmdSerial.available()) 
  {  
    current = m_cmdSerial.read();

    if(current < 'a') // if uppercase
    {
      current += DIFF_LOWER_UPPER;  // make it lower case
    }
    
    command = input_to_command(current);
  }

  return command; 
  
}

bool BLEJoystickDecoder::check_external_command(int &desired_speed, int &desired_servo_angle)
{
    auto incoming_cmd = ovladacka_decoder();

    if(incoming_cmd != RobotCommand::NO_COMMAND)
    {
      switch(incoming_cmd)
      {
        case RobotCommand::UP:
          if(desired_speed < MAX_SPD)
          {
            desired_speed += SPD_INCREMENT;
          }
        break;
        case RobotCommand::DOWN:
          if(desired_speed > -MAX_SPD)
          {
            desired_speed -= SPD_INCREMENT;
          }
        break;
        case RobotCommand::RIGHT:
          if(desired_servo_angle > SERVO_MIN_RIGHT)
          {
            desired_servo_angle -= SERVO_INCREMENT;
          }
        break;
        case RobotCommand::LEFT:
          if(desired_servo_angle < SERVO_MAX_LEFT)
          {
            desired_servo_angle += SERVO_INCREMENT;
          }
        break;
        case RobotCommand::UP_TRIANGLE:
          desired_servo_angle = SERVO_CENTER;
        break;
        /*case RIGHT_CIRCLE:
          //
        break;*/
        default:
          desired_speed = 0; 
      }
    }
    else
    {
      return false; 
    }

    return true; 

}
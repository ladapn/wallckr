#include "ExternalCommandDecoder.h"
#include "MotionConstants.h"
#include "RobotCommand.h"
#include <Arduino.h>

JoystickCommand ExternalCommandDecoder::input_to_joystick_command(char in)
{
  JoystickCommand command = JoystickCommand::NO_COMMAND;

  switch(in)
  {
    case 'a':
      command = JoystickCommand::UP;
    break;
    case 'b':
      command = JoystickCommand::RIGHT;
    break;
    case 'c':
      command = JoystickCommand::DOWN;
    break;
    case 'd':
      command = JoystickCommand::LEFT;
    break;
    case 'e':
      command = JoystickCommand::UP_TRIANGLE;
    break;
    case 'f':
      command = JoystickCommand::RIGHT_CIRCLE;
    break;
    case 'g':
      command = JoystickCommand::DOWN_X;
    break;
    case 'h':
      command = JoystickCommand::LEFT_SQUARE;
    break;

  }

  return command; 
}

// decoding commands from BLE joystick Iphone app which sends commands in
// format "Aa\0" -> Upper case, lower case, zero for each button press ->
// buttons are labeled from A to H
JoystickCommand ExternalCommandDecoder::BLE_joystick_decoder()
{
  JoystickCommand command = JoystickCommand::NO_COMMAND; 
  char current, prev;
  bool waiting4zero = false;
  const int DIFF_LOWER_UPPER = 'a' - 'A';

  while(!m_cmdSerial.available())
  {
    ;
  }

  prev = m_cmdSerial.read();

  while(command == JoystickCommand::NO_COMMAND) 
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
        command = input_to_joystick_command(prev);
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

JoystickCommand ExternalCommandDecoder::ovladacka_decoder()
{
  JoystickCommand command = JoystickCommand::NO_COMMAND; 
  char current;
  const int DIFF_LOWER_UPPER = 'a' - 'A';

  if(m_cmdSerial.available()) 
  {  
    current = m_cmdSerial.read();

    if(current < 'a') // if uppercase
    {
      current += DIFF_LOWER_UPPER;  // make it lower case
    }
    
    command = input_to_joystick_command(current);
  }

  return command; 
  
}

bool ExternalCommandDecoder::check_external_command(RobotCommand &robot_command)
{
    auto incoming_cmd = ovladacka_decoder();

    if(incoming_cmd != JoystickCommand::NO_COMMAND)
    {
      switch(incoming_cmd)
      {
        case JoystickCommand::UP:
          if(robot_command.desired_speed < MAX_SPD)
          {
            robot_command.desired_speed += SPD_INCREMENT;
          }
        break;
        case JoystickCommand::DOWN:
          if(robot_command.desired_speed > -MAX_SPD)
          {
            robot_command.desired_speed -= SPD_INCREMENT;
          }
        break;
        case JoystickCommand::RIGHT:
          if(robot_command.desired_servo_angle > SERVO_MIN_RIGHT)
          {
            robot_command.desired_servo_angle -= SERVO_INCREMENT;
          }
        break;
        case JoystickCommand::LEFT:
          if(robot_command.desired_servo_angle < SERVO_MAX_LEFT)
          {
            robot_command.desired_servo_angle += SERVO_INCREMENT;
          }
        break;
        case JoystickCommand::UP_TRIANGLE:
          robot_command.desired_servo_angle = SERVO_CENTER;
        break;
        case JoystickCommand::RIGHT_CIRCLE:
          robot_command.enable_automatic_operation = !robot_command.enable_automatic_operation;
        break;
        default:
          robot_command.desired_speed = 0; 
      }
    }
    else
    {
      return false; 
    }

    return true; 

}
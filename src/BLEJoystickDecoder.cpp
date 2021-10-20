#include "BLEJoystickDecoder.h"
#include "Arduino.h"
#include "Motion.h"

command_t BLEJoystickDecoder::input_to_command(char in)
{
  command_t command = NO_COMMAND;

  switch(in)
  {
    case 'a':
      command = UP;
    break;
    case 'b':
      command = RIGHT;
    break;
    case 'c':
      command = DOWN;
    break;
    case 'd':
      command = LEFT;
    break;
    case 'e':
      command = UP_TRIANGLE;
    break;
    case 'f':
      command = RIGHT_CIRCLE;
    break;
    case 'g':
      command = DOWN_X;
    break;
    case 'h':
      command = LEFT_SQUARE;
    break;

  }

  return command; 
}

// decoding commands from BLE joystick Iphone app which sends commands in
// format "Aa\0" -> Upper case, lower case, zero for each button press ->
// buttons are labled from A to H
command_t BLEJoystickDecoder::command_decoder()
{
  command_t command = NO_COMMAND; 
  char current, prev;
  bool waiting4zero = false;
  const int DIFF_LOWER_UPPER = 'a' - 'A';

  while(!m_cmdSerial.available())
  {
    ;
  }

  prev = m_cmdSerial.read();

  while(command == NO_COMMAND) 
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

command_t BLEJoystickDecoder::command_decoder_fast()
{
  command_t command = NO_COMMAND; 
  char current, prev;
  const int DIFF_LOWER_UPPER = 'a' - 'A';

  while(!m_cmdSerial.available())
  {
    ;
  }

  prev = m_cmdSerial.read();

  while(command == NO_COMMAND) 
  {
    while(!m_cmdSerial.available())
    {
      ;
    }
    
    current = m_cmdSerial.read();
    
    if(current - prev == DIFF_LOWER_UPPER) 
    {
      command = input_to_command(current);
    }

    prev = current; 
  }

  return command; 
  
}

command_t BLEJoystickDecoder::command_decoder_fastest()
{
  command_t command = NO_COMMAND; 
  char current;
  const int DIFF_LOWER_UPPER = 'a' - 'A';

  if(m_cmdSerial.available()) 
  {  
    current = m_cmdSerial.read();

    if(current < 'a') // if upperacase
    {
      current += DIFF_LOWER_UPPER;  // make it lower case
    }
    
    command = input_to_command(current);
  }

  return command; 
  
}

bool BLEJoystickDecoder::check_motion_cmd(int &desired_speed, int &desired_servo_angle)
{
    command_t incomming_cmd = command_decoder_fastest();

    if(incomming_cmd != NO_COMMAND)
    {
      switch(incomming_cmd)
      {
        case UP:
          if(desired_speed < MAX_SPD)
          {
            desired_speed += SPD_INCREMENT;
          }
        break;
        case DOWN:
          if(desired_speed > -MAX_SPD)
          {
            desired_speed -= SPD_INCREMENT;
          }
        break;
        case RIGHT:
          if(desired_servo_angle > SERVO_MIN_RIGHT)
          {
            desired_servo_angle -= SERVO_INCREMENT;
          }
        break;
        case LEFT:
          if(desired_servo_angle < SERVO_MAX_LEFT)
          {
            desired_servo_angle += SERVO_INCREMENT;
          }
        break;
        case UP_TRIANGLE:
          desired_servo_angle = SERVO_CENTER;
        break;
        default:
          desired_speed = 0; 
      }
    }

    return true; 

}
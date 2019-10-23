#include "BLEJoystickDecoder.h"
#include "Arduino.h"

command_t select_command(char in)
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

  }

  return command; 
}

// decoding commands from BLE joystick Iphone app which sends commands in
// format "Aa\0" -> Upper case, lower case, zero for each button press ->
// buttons are labled from A to H
command_t command_decoder(HardwareSerial &cmdSerial)
{
  command_t command = NO_COMMAND; 
  char current, prev;
  bool waiting4zero = false;
  const int DIFF_LOWER_UPPER = 'a' - 'A';

  while(!cmdSerial.available())
  {
    ;
  }

  prev = cmdSerial.read();

  while(command == 0) 
  {
    while(!cmdSerial.available())
    {
      ;
    }
    
    current = cmdSerial.read();

    if(waiting4zero)
    {

      
      if(current == 0)
      {
        command = select_command(prev);
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
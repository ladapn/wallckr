#ifndef BLEJOYSTICKDECODER_H
#define BLEJOYSTICKDECODER_H

#include "Arduino.h"

typedef enum cmd {NO_COMMAND = 0, UP, RIGHT, DOWN, LEFT } command_t; 

command_t select_command(char in);
command_t command_decoder(HardwareSerial &cmdSerial);

#endif // BLEJOYSTICKDECODER_H
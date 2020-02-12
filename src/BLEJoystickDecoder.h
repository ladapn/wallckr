#ifndef BLEJOYSTICKDECODER_H
#define BLEJOYSTICKDECODER_H

#include "Arduino.h"

typedef enum cmd {NO_COMMAND = 0, UP, RIGHT, DOWN, LEFT, UP_TRIANGLE, RIGHT_CIRCLE, DOWN_X, LEFT_SQUARE } command_t; 

command_t select_command(char in);
command_t command_decoder(HardwareSerial &cmdSerial);
command_t command_decoder_fast(HardwareSerial &cmdSerial);
command_t command_decoder_fastest(HardwareSerial &cmdSerial);

#endif // BLEJOYSTICKDECODER_H
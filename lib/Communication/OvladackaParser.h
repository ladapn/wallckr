#ifndef OVLADACKA_PARSER_H
#define OVLADACKA_PARSER_H

#include "InputStreamParser.h"
#include <Stream.h>


class OvladackaParser : public InputStreamParser
{
public:
    JoystickCommand decode(Stream &input_stream) override
    {
        JoystickCommand command = JoystickCommand::NO_COMMAND; 
        char current;
        const int DIFF_LOWER_UPPER = 'a' - 'A';

        if(input_stream.available()) 
        {  
            current = input_stream.read();

            if(current < 'a') // if uppercase
            {
            current += DIFF_LOWER_UPPER;  // make it lower case
            }
            
            command = input_to_joystick_command(current);
        }

        return command; 
    };

};

#endif
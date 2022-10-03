#ifndef OVLADACKA_PARSER_H
#define OVLADACKA_PARSER_H

#include "InputStreamParser.h"
#include <Stream.h>

/**
 * Implementation of communication protocol used by ovladacka tool
 */
class OvladackaParser : public InputStreamParser
{
public:
    /**
     * Decode input format into JoysticCommand format
     * @param input_stream stream to receive input data from
     * @return decoded command in JoysticCommand format, or JoystickCommand::NO_COMMAND 
     * if no command was present
     */
    JoystickCommand decode(Stream &input_stream) override
    {
        JoystickCommand command = JoystickCommand::NO_COMMAND; 
        const int DIFF_LOWER_UPPER = 'a' - 'A';

        if(input_stream.available()) 
        {  
            auto current = input_stream.read();

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
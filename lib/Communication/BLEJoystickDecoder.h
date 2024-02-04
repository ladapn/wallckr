#ifndef BLEJOYSTICK_DECODER_H
#define BLEJOYSTICK_DECODER_H

#include "InputStreamParser.h"
#include "IRobotIOStream.h"

/**
 * Implementation of communication protocol used by BLEJoystick application,
 * which sends commands in format "Aa\0" -> Upper case, lower case, 
 * zero for each button press -> buttons are labeled from A to H
 */ 
class BLEJoystickDecoder : public InputStreamParser
{
    /**
     * Decode input format into JoysticCommand format
     * @param input_stream stream to receive input data from
     * @return decoded command in JoysticCommand format, or JoystickCommand::NO_COMMAND 
     * if no command was present
     */
    JoystickCommand decode(IRobotIOStream &input_stream) override
    {
        JoystickCommand command = JoystickCommand::NO_COMMAND; 
        char current, prev;
        bool waiting4zero = false;
        const int DIFF_LOWER_UPPER = 'a' - 'A';

        // TODO: remove this active waiting, just check it and go
        while(!input_stream.available())
        {
            ;
        }

        prev = input_stream.read();

        while(command == JoystickCommand::NO_COMMAND) 
        {
            while(!input_stream.available())
            {
            ;
            }
            
            current = input_stream.read();

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
    
    };
};

#endif 
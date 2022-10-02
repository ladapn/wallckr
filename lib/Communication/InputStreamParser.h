#ifndef INPUT_STREAM_PARSER_H
#define INPUT_STREAM_PARSER_H

class Stream;

enum class JoystickCommand { NO_COMMAND = 0, UP, RIGHT, DOWN, LEFT, UP_TRIANGLE, RIGHT_CIRCLE, DOWN_X, LEFT_SQUARE }; 

class InputStreamParser
{

public:
    virtual JoystickCommand decode(Stream &input_stream) = 0;

protected: 
    JoystickCommand input_to_joystick_command(char in)
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
    };
};



class BLEJoystickParser : public InputStreamParser
{
public:
    JoystickCommand decode(Stream &input_stream) override;

};

#endif  //INPUT_STREAM_PARSER_H
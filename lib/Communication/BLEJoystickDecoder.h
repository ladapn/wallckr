#ifndef BLEJOYSTICKDECODER_H
#define BLEJOYSTICKDECODER_H

class Stream; 

enum command_t {NO_COMMAND = 0, UP, RIGHT, DOWN, LEFT, UP_TRIANGLE, RIGHT_CIRCLE, DOWN_X, LEFT_SQUARE }; 

class BLEJoystickDecoder
{
    Stream &m_cmdSerial;
    command_t input_to_command(char in);
    command_t command_decoder();
    command_t command_decoder_fast();
    command_t command_decoder_fastest();

public: 

    explicit BLEJoystickDecoder(Stream &cmdSerial) : m_cmdSerial(cmdSerial) {};
    bool check_motion_cmd(int &desired_speed, int &desired_servo_angle);
};
#endif // BLEJOYSTICKDECODER_H
#ifndef EXTERNAL_COMMAND_DECODER_H
#define EXTERNAL_COMMAND_DECODER_H

class Stream;
struct RobotCommand; 

enum class JoystickCommand { NO_COMMAND = 0, UP, RIGHT, DOWN, LEFT, UP_TRIANGLE, RIGHT_CIRCLE, DOWN_X, LEFT_SQUARE }; 

class ExternalCommandDecoder // ExternalCommandDecoder
{
    Stream &m_cmdSerial;
    // InputStreamParser &input_decoder
    JoystickCommand input_to_joystick_command(char in);
    JoystickCommand BLE_joystick_decoder();
    JoystickCommand ovladacka_decoder();
    

public: 

    /**
     * Constructor method
     * @param[in] cmdSerial stream used as a source for incoming commands
     */
    explicit ExternalCommandDecoder(Stream &cmdSerial) : m_cmdSerial(cmdSerial) {};
    /**
     * Check if external command is present, if so, parse it and update impacted field of robot_command structure
     * @param[out] robot_command structure to hold extracted robot command (desired speed, steering angle etc)
     * @return true if there was a command and thus if any of the fields were updated, false otherwise
     */
    bool check_external_command(RobotCommand &robot_command);
};
#endif // EXTERNAL_COMMAND_DECODER_H
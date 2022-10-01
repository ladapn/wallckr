#ifndef BLEJOYSTICKDECODER_H
#define BLEJOYSTICKDECODER_H

class Stream;
struct RobotCommand; 

enum class JoystickCommand { NO_COMMAND = 0, UP, RIGHT, DOWN, LEFT, UP_TRIANGLE, RIGHT_CIRCLE, DOWN_X, LEFT_SQUARE }; 

class BLEJoystickDecoder
{
    Stream &m_cmdSerial;
    JoystickCommand input_to_command(char in);
    JoystickCommand BLE_joystick_decoder();
    JoystickCommand ovladacka_decoder();

public: 

    explicit BLEJoystickDecoder(Stream &cmdSerial) : m_cmdSerial(cmdSerial) {};
    /**
     * Check if external command is present, if so, parse it and update impacted field of robot_command structure
     * @param[out] robot_command structure to hold extracted robot command (desired speed, steering angle etc)
     * @return true if there was a command and thus if any of the fields were updated, false otherwise
     */
    bool check_external_command(RobotCommand &robot_command);
};
#endif // BLEJOYSTICKDECODER_H
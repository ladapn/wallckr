#ifndef EXTERNAL_COMMAND_DECODER_H
#define EXTERNAL_COMMAND_DECODER_H

class Stream;
class InputStreamParser;
struct RobotCommand; 


class ExternalCommandDecoder
{

    Stream &m_cmdSerial;
    InputStreamParser &input_decoder;
    

public: 
    /**
     * Constructor method
     * @param[in] cmdSerial stream used as a source for incoming commands
     */
    explicit ExternalCommandDecoder(Stream &cmdSerial, InputStreamParser &in_parser) : m_cmdSerial(cmdSerial), input_decoder(in_parser) {};
    /**
     * Check if external command is present, if so, parse it and update impacted field of robot_command structure
     * @param[out] robot_command structure to hold extracted robot command (desired speed, steering angle etc)
     * @return true if there was a command and thus if any of the fields were updated, false otherwise
     */
    bool check_external_command(RobotCommand &robot_command);
};
#endif // EXTERNAL_COMMAND_DECODER_H
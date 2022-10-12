#ifndef ROBOT_PACKETS_H
#define ROBOT_PACKETS_H

#include <stdlib.h>
#include <stdint.h>

/**
 * enum defining all possible IDs of ultrasound measurement packets 
 */
enum us_packet_ids {FRONT_US_ID = 100, RIGHT_FRONT_US_ID = 101, RIGHT_CENTER_US_ID = 102, RIGHT_BACK_US_ID = 103};
/**
 * enum defining all possible IDs of utility packets 
 */
enum util_packet_ids {STATUS_ID = 80, MOTION_CMD_ID = 90};

/**
 * Structure representing output packet to hold measurements from ultrasound sensors 
 */
struct SonarPacket {
    uint8_t id;
    unsigned long tick;
    unsigned long sonar_data;
    uint8_t crc = 0;
    /**
     * Constructor method
     * @param[in] packet_id ID of ultrasound packet, it differs based on sensor location - see \ref us_packet_ids
     * @param[in] tick_ms current system time in ms
     * @param[in] measurement ultrasound measurement 
     */
    SonarPacket(uint8_t packet_id, unsigned long tick_ms, unsigned long measurement) :
    id(packet_id),
    tick(tick_ms),
    sonar_data(measurement)
    {
    }
};

/**
 * Structure representing status packet - packet holding information about robot's status 
 */
struct StatusPacket {
    uint8_t id = STATUS_ID;
    unsigned long tick;
    unsigned long version_ID = GIT_REV; // long -> 4 bytes, instead of 8 chars
    unsigned int battery_voltage_adc; // analogRead() returns int; int is 2 bytes long on ATmega
    unsigned int total_current_adc;
    unsigned int motor_current_adc;
    uint8_t crc = 0;

    /**
     * Constructor method
     * @param[in] tick_ms current system time in ms
     * @param[in] battery_voltage current battery voltage in ADC units
     * @param[in] total_current total current drawn from battery in mA
     * @param[in] motor_current total current drawn by motor in mA 
     */
    StatusPacket(unsigned long tick_ms, unsigned int battery_voltage, unsigned int total_current, unsigned int motor_current) :
    tick(tick_ms),
    battery_voltage_adc(battery_voltage),
    total_current_adc(total_current),
    motor_current_adc(motor_current)
    {
    }

};

/**
 * Structure representing motion command packet -> packet sent back when a motion command is received from user 
 */
struct MotionCommandPacket {
    uint8_t id = MOTION_CMD_ID;
    unsigned long tick;
    unsigned int servo_cmd;
    signed int motor_cmd; 
    uint8_t crc = 0;
    /**
     * Constructor method
     * @param[in] tick_ms current system time in ms
     * @param[in] servo_command desired servo angle in deg
     * @param[in] motor_command desired motor speed on scale from -255 to 255, where 255 is full speed
     * and negative numbers mean going backward 
    */
    MotionCommandPacket(unsigned long tick_ms, unsigned int servo_command, signed int motor_command) :
    tick(tick_ms),
    servo_cmd(servo_command),
    motor_cmd(motor_command)
    {}
};

class Stream;

/**
 * Class to send out robot packets 
 */
class RobotPrinter
{
    Stream &robot_serial;
    /**
     * Compute CRC by xoring all bytes together 
     * @param[in] buffer chunk of data containing packet
     * @param[in] size size of packet in bytes
     * @return computed CRC 
     */
    uint8_t compute_xor_CRC(const uint8_t *buffer, size_t size);
public:
    /**
     * Constructor method
     * @param[in] serial serial interface packets are sent to
     */
    explicit RobotPrinter(Stream &serial) : robot_serial(serial) {};
    /**
     * Compute packet's CRC and send it out
     * @param[in] buffer chunk of data containing packet 
     * @param[in] size size of packet in bytes
     * @return number of bytes written 
     */    
    size_t add_CRC_and_print(uint8_t *buffer, size_t size);

};

#endif
#ifndef BLEPACKETS_H
#define BLEPACKETS_H

#include <stdlib.h>
#include <stdint.h>

enum us_packet_ids {FRONT_US_ID = 100, RIGHT_FRONT_US_ID = 101, RIGHT_CENTER_US_ID = 102, RIGHT_BACK_US_ID = 103};
enum util_packet_ids {STATUS_ID = 80, MOTION_CMD_ID = 90};

struct SonarPacket {
    uint8_t id;
    unsigned long tick;
    unsigned long sonar_data;
    uint8_t crc = 0;
    SonarPacket(uint8_t packet_id, unsigned long tick_ms, unsigned long measurement) :
    id(packet_id),
    tick(tick_ms),
    sonar_data(measurement)
    {
    }
};

// status packet, battery voltage, current, motor current? version ID?
struct StatusPacket {
    uint8_t id = STATUS_ID;
    unsigned long tick;
    unsigned long version_ID = GIT_REV; // long -> 4 bytes, instead of 8 chars
    unsigned int battery_voltage_adc; // analogRead() returns int; int is 2 bytes long on ATmega
    unsigned int total_current_adc;
    unsigned int motor_current_adc;
    uint8_t crc = 0;

    StatusPacket(unsigned long tick_ms, unsigned int battery_voltage, unsigned int total_current, unsigned int motor_current) :
    tick(tick_ms),
    battery_voltage_adc(battery_voltage),
    total_current_adc(total_current),
    motor_current_adc(motor_current)
    {
    }

};

// motion command packet - srv and spd command
struct MotionCommandPacket {
    uint8_t id = MOTION_CMD_ID;
    unsigned long tick;
    unsigned int servo_cmd;
    signed int motor_cmd; 
    uint8_t crc = 0;

};

class Stream;

class RobotPrinter
{
    Stream &robot_serial;
    uint8_t compute_xor_CRC(const uint8_t *buffer, size_t size);
public:
    explicit RobotPrinter(Stream &serial) : robot_serial(serial) {};    
    size_t add_CRC_and_print(uint8_t *buffer, size_t size);

};

#endif
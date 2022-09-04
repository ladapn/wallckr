#ifndef BLEPACKETS_H
#define BLEPACKETS_H

#include <stdlib.h>
#include <stdint.h>

enum us_packet_ids {FRONT_US_ID = 100, RIGHT_FRONT_US_ID = 101, RIGHT_CENTER_US_ID = 102, RIGHT_BACK_US_ID = 103};
enum util_packet_ids {STATUS_ID = 80, MOTION_CMD_ID = 90};

// TODO: base class that provides virtual print, each packet will know how to print itself
/*

class robot_packet {
    public:
    virtual size_t print() = 0;
}

class sonar_packet {
    public:
    sonar_packet(byte id, unsigned long tick, unsigned long sonar_data)
    size_t print() override
    {
        // can this be common for all packets? ... this, sizeof(*this)???
        // but wait a minute, sizeof is evaluated at compile time so I cannot call it from the base class (it would always return the size of the base class)
        return m_BLE_serial.write((uint8_t*)&us_data, sizeof(us_data));
    }
}

*/


struct sonar_packet_t {
    uint8_t id;
    unsigned long tick;
    unsigned long sonar_data;
    uint8_t crc;
    };

// status packet, battery voltage, current, motor current? version ID?
struct status_packet_t {
    uint8_t id;
    unsigned long tick;
    unsigned long version_ID = GIT_REV; // long -> 4 bytes, instead of 8 chars
    unsigned int battery_voltage_adc; // analogRead() returns int; int is 2 bytes long on ATmega
    unsigned int total_currnet_adc;
    unsigned int motor_currnet_adc;
    uint8_t crc;

    };

// motion command packet - srv and spd command
struct motion_command_packet_t {
    uint8_t id;
    unsigned long tick;
    unsigned int servo_cmd; // int appropriate here? 
    signed int motor_cmd; // int appropriate here? 
    uint8_t crc;

};

class Stream;

class BLE_printer
{
    Stream &m_BLE_serial;
public:
    BLE_printer(Stream &serial) : m_BLE_serial(serial) {};
    size_t BLE_print_US_data(uint8_t id, unsigned long tick, unsigned long sonar_data);
    size_t BLE_print_status_data(uint8_t id, unsigned long tick, unsigned int battery_adc);
    uint8_t compute_xor_CRC(const uint8_t *buffer, size_t size);

};

#endif
#ifndef BLEPACKETS_H
#define BLEPACKETS_H

#include <Arduino.h>

enum us_packet_ids {FRONT_US_ID = 100, RIGHT_FRONT_US_ID = 101, RIGHT_CENTER_US_ID = 102, RIGHT_BACK_US_ID = 103};
enum util_packet_ids {STATUS_ID = 80, MOTION_CMD_ID = 90};

struct sonar_packet_t {
    byte id;
    unsigned long tick;
    unsigned long sonar_data;
    byte crc;
    };

// status packet, battery voltage, current, motor current? version ID?
struct status_packet_t {
    byte id;
    unsigned long tick;
    unsigned long version_ID; // long -> 4 bytes, instead of 8 chars
    unsigned int battery_voltage_adc; // analogRead() returns int; int is 2 bytes long on ATmega
    unsigned int total_currnet_adc;
    unsigned int motor_currnet_adc;
    byte crc;

    // FIXME probably not the best way of doing it
    status_packet_t()
    {
        version_ID = GIT_REV;
    };
    };

// motion command packet - srv and spd command
struct motion_command_packet_t {
    byte id;
    unsigned long tick;
    unsigned int servo_cmd; // int appropriate here? 
    signed int motor_cmd; // int appropriate here? 
    byte crc;

};


class BLE_printer
{
    HardwareSerial &m_BLE_serial;
public:
    BLE_printer(HardwareSerial &serial) : m_BLE_serial(serial) {};
    size_t BLE_print_US_data(byte id, unsigned long tick, unsigned long sonar_data);
    size_t BLE_print_status_data(byte id, unsigned long tick, unsigned int battery_adc);
    byte compute_xor_CRC(const uint8_t *buffer, size_t size);

};


#endif
#ifndef BLEPACKETS_H
#define BLEPACKETS_H

#include <Arduino.h>

enum us_packet_ids {FRONT_US_ID = 100, RIGHT_FRONT_US_ID = 101, RIGHT_CENTER_US_ID = 102, RIGHT_BACK_US_ID = 103};

typedef struct sonar_packet{
    byte id;
    unsigned long tick;
    unsigned long sonar_data;
    byte crc;
    } sonar_packet_t;

// status packet, battery voltage, current, motor current? version ID?


class BLE_printer
{
    HardwareSerial &m_BLE_serial;
public:
    BLE_printer(HardwareSerial &serial) : m_BLE_serial(serial) {};
    size_t BLE_print_US_data(byte id, unsigned long tick, unsigned long sonar_data);

};


#endif
#ifndef BLEPACKETS_H
#define BLEPACKETS_H

#include <Arduino.h>

typedef struct sonar_packet{
    byte id;
    unsigned long tick;
    unsigned long sonar_data;
    byte crc;
    } sonar_packet_t;

#endif
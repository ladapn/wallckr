// TODO includion guard
#include <Arduino.h>

typedef struct sonar_packet{
    byte id;
    unsigned long tick;
    unsigned long sonar_data;
    byte crc;
    } sonar_packet_t;
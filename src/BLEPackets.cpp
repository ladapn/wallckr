#include "BLEPackets.h"

size_t BLE_printer::BLE_print_US_data(byte id, unsigned long tick, unsigned long sonar_data)
{
    sonar_packet_t us_data;

    us_data.id = id;
    us_data.sonar_data = sonar_data;
    us_data.tick = tick;
    
    // TODO add CRC computation
    us_data.crc = 0;
    
    return m_BLE_serial.write((uint8_t*)&us_data, sizeof(us_data));
}
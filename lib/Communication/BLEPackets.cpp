#include "BLEPackets.h"
#include "Arduino.h"

size_t BLE_printer::BLE_print_US_data(uint8_t id, unsigned long tick, unsigned long sonar_data)
{
    SonarPacket us_data;

    us_data.id = id;
    us_data.sonar_data = sonar_data;
    us_data.tick = tick;
    
    // TODO add CRC_and_send function
    us_data.crc = compute_xor_CRC((uint8_t*)&us_data, sizeof(us_data));
    
    return m_BLE_serial.write((uint8_t*)&us_data, sizeof(us_data));
}

size_t BLE_printer::BLE_print_status_data(uint8_t id, unsigned long tick, unsigned int battery_adc)
{
    StatusPacket status_data;

    status_data.id = id;
    status_data.tick = tick;
    
    status_data.battery_voltage_adc = battery_adc;
    status_data.total_currnet_adc = 0; // FIXME
    status_data.motor_currnet_adc = 0; // FIXME

    // TODO add CRC computation
    status_data.crc = compute_xor_CRC((uint8_t*)&status_data, sizeof(status_data));
    
    return m_BLE_serial.write((uint8_t*)&status_data, sizeof(status_data));
}

// TODO: verify this does not introduce too much delay to control loop
uint8_t BLE_printer::compute_xor_CRC(const uint8_t *buffer, size_t size)
{     
    uint8_t CRC = 0;

    if(size)
    {
        CRC = buffer[0];

        // (size - 1) -> don't include the last byte - the CRC byte
        for(unsigned int i = 1; i < (size - 1); i++)
        {
            CRC ^= buffer[i];
        }
    }

    return CRC;
}
#include "RobotPackets.h"
#include "Arduino.h"

uint8_t RobotPrinter::compute_xor_CRC(const uint8_t *buffer, size_t size)
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

size_t RobotPrinter::add_CRC_and_print(uint8_t *buffer, size_t size)
{    

    buffer[size - 1] = compute_xor_CRC(buffer, size);
    return robot_serial.write(buffer, size);
}
#ifndef BLESERIAKSTREAM_H
#define BLESERIAKSTREAM_H

#include "IRobotIOStream.h"

class BLESerialStream: public IRobotIOStream {


public:
    explicit BLESerialStream() {}
    int available() override
    {
        // return number of chars in input buffer
    }

    int read() override
    {
        // return one char from the input buffer 
    }

    size_t write(uint8_t c) override
    {
        // Notify robot output characteristics
        // Maybe just write(&c, 1);? 
    }

    size_t write(uint8_t *c, size_t size) override
    {
        // Notify robot output characteristics
    }
};

#endif // BLESERIAKSTREAM_H
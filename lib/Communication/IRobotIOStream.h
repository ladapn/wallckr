#ifndef IROBOTIOSTREAM_H
#define IROBOTIOSTREAM_H

#include <stdint.h>
#include <stddef.h>

class IRobotIOStream
{

public:
    virtual int available() = 0;
    virtual int read() = 0;
    virtual size_t write(uint8_t c) = 0;
    virtual size_t write(uint8_t *c, size_t size) = 0;
};

#endif
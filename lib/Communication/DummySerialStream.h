#ifndef DUMMYSERIALSTREAM_H
#define DUMMYSERIALSTREAM_H

#include <stdint.h>
#include <stddef.h>
#include "IRobotIOStream.h"

class DummySerialStream : public IRobotIOStream
{

public:
    virtual int available() { return 0; };
    virtual int read() { return 0; };
    virtual size_t write(uint8_t c) { return 0; };
    virtual size_t write(uint8_t *c, size_t size) { return 0; };
};

#endif  // DUMMYSERIALSTREAM_H
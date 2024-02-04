#ifndef ARDUINOSERIAKSTREAM_H
#define ARDUINOSERIAKSTREAM_H

#include <Arduino.h>
#include "IRobotIOStream.h"

class ArduinoSerialStream: public IRobotIOStream {
    Stream &serial_stream;

public:
    explicit ArduinoSerialStream(Stream &stream) : serial_stream(stream) {}
    int available() override
    {
        return serial_stream.available();
    }

    int read() override
    {
        return serial_stream.read();
    }

    size_t write(uint8_t c) override
    {
        return serial_stream.write(c);
    }

    size_t write(uint8_t *c, size_t size) override
    {
        return serial_stream.write(c, size);
    }
};

#endif // ARDUINOSERIAKSTREAM_H
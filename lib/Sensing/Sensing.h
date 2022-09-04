#ifndef SENSING_H
#define SENSING_H

#include "UltraSoundSensor.h"

class Sensing
{
    // battery related constants 
    static const int BATTERY_CELL_COUNT = 8;
    static const int ADC_REFERENCE_V = 5;
    static const int ADC_MAX = 1023; 
    static const int VOLTAGE_DIVIDER_FACTOR = 16;
    static constexpr float BATTERY_CELL_CUTTOFF_V = 0.9f;
    static const int BATTERY_PACK_CUTTOFF_ADC = (BATTERY_CELL_CUTTOFF_V / ADC_REFERENCE_V * ADC_MAX * BATTERY_CELL_COUNT) / VOLTAGE_DIVIDER_FACTOR;
    static const int SNS_BATTERY_VLTG = A8;

    // sonar related constants
    static const int TRIGGER_PIN_FRONT = 32;  // Arduino pin tied to trigger pin on the ultrasonic sensor.
    static const int ECHO_PIN_FRONT = 33;  // Arduino pin tied to echo pin on the ultrasonic sensor.

    static const int TRIGGER_PIN_RIGHT_FRONT = 34;
    static const int ECHO_PIN_RIGHT_FRONT = 35;

    static const int TRIGGER_PIN_RIGHT_CENTER = 36;
    static const int ECHO_PIN_RIGHT_CENTER = 37;

    static const int MAX_DISTANCE = 200; // TODO: rename to MAX_DISTANCE_CM

    UltraSoundSensor sonar_front; //(TRIGGER_PIN_FRONT, ECHO_PIN_FRONT, MAX_DISTANCE);
    UltraSoundSensor sonar_right_front; //(TRIGGER_PIN_RIGHT_FRONT, ECHO_PIN_RIGHT_FRONT, MAX_DISTANCE);
    UltraSoundSensor sonar_right_center; //(TRIGGER_PIN_RIGHT_CENTER, ECHO_PIN_RIGHT_CENTER, MAX_DISTANCE);

public:
    // TODO reference to printer? 
    Sensing() : sonar_front(TRIGGER_PIN_FRONT, ECHO_PIN_FRONT, MAX_DISTANCE),
                sonar_right_front(TRIGGER_PIN_RIGHT_FRONT, ECHO_PIN_RIGHT_FRONT, MAX_DISTANCE),
                sonar_right_center(TRIGGER_PIN_RIGHT_CENTER, ECHO_PIN_RIGHT_CENTER, MAX_DISTANCE) {};
    
    bool battery_voltage_ok(); 
    unsigned int get_side_distance_cm();
    unsigned int get_front_distance_cm();

};

#endif // SENSING_H
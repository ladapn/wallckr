#ifndef SENSING_H
#define SENSING_H

#include "UltraSoundSensor.h"

class BLE_printer;

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

    IDistanceSensor &sensor_front; 
    IDistanceSensor &sensor_right_front; 
    IDistanceSensor &sensor_right_center;

    BLE_printer &sensor_printer; 

    bool disabled;

public:
    // TODO reference to printer? 
    Sensing(IDistanceSensor &front, IDistanceSensor &right_front, IDistanceSensor &right_center, BLE_printer &printer) : 
                sensor_front(front),
                sensor_right_front(right_front),
                sensor_right_center(right_center),
                sensor_printer(printer),
                disabled(false) {};
    
    bool battery_voltage_ok(long currentMillis);
    unsigned int get_side_distance_cm(long currentMillis, unsigned long &right_front_distance_cm, unsigned long &right_center_distance_cm);
    unsigned int get_front_distance_cm(long currentMillis);
    void disable()
    {
        disabled = true; 
    };
    void enable()
    {
        disabled = false;
    };

};

#endif // SENSING_H
#ifndef SENSING_H
#define SENSING_H

#include "UltraSoundSensor.h"

class RobotPrinter;

/**
 * Structure grouping all possible types of distance measurements, namely 
 * right front distance (45 deg from front direction),
 * right center distance (90 deg from front direction),
 * right distance (minimum value of all right distances),
 * front distance
 */
struct DistanceMeasurements
{
    unsigned long right_distance_cm = 0;
    unsigned long right_front_distance_cm = 0;
    unsigned long right_center_distance_cm = 0;
    unsigned long front_distance_cm = 0;

};

/**
 * Class encapsulating all measurement devices
 */
class Sensing
{
    // battery related constants 
    static const int BATTERY_CELL_COUNT = 8;
    static const int ADC_REFERENCE_V = 5;
    static const int ADC_MAX = 1023; 
    static const int VOLTAGE_DIVIDER_FACTOR = 16;
    static constexpr float BATTERY_CELL_CUTTOFF_V = 0.9f;
    static const int BATTERY_PACK_CUTTOFF_ADC = (BATTERY_CELL_CUTTOFF_V / ADC_REFERENCE_V * ADC_MAX * BATTERY_CELL_COUNT) / VOLTAGE_DIVIDER_FACTOR;
    //static const int SNS_BATTERY_VLTG = A8;
    static const int SNS_BATTERY_VLTG = 0;

    IDistanceSensor &sensor_front; 
    IDistanceSensor &sensor_right_front; 
    IDistanceSensor &sensor_right_center;

    RobotPrinter &sensor_printer; 

    bool disabled;

public:
    /**
     * Constructor method
     * @param[in] front reference to front distance sensor
     * @param[in] right_front reference to right front sensor (45 deg from front direction)
     * @param[in] right_center reference to right center sensor (90 deg from front direction)
     * @param[in] printer reference to serial interface which is used to send out measured data 
     */
    Sensing(IDistanceSensor &front, IDistanceSensor &right_front, IDistanceSensor &right_center, RobotPrinter &printer) : 
                sensor_front(front),
                sensor_right_front(right_front),
                sensor_right_center(right_center),
                sensor_printer(printer),
                disabled(false) {};
    
    /**
     * Check if battery voltage is above threshold (BATTERY_PACK_CUTTOFF_ADC), also obtained measurement is 
     * sent out via serial interface
     * @param[in] current_millis current system tick
     * @return true if battery voltage is above threshold, false otherwise
     */ 
    bool battery_voltage_ok(long current_millis);
    /**
     * Measure side distance to wall or obstacle, also obtained measurements are sent out via serial interface
     * @param[in] current_millis current system tick
     * @param[out] right_front_distance_cm right front (45 deg from front direction) distance in cm
     * @param[out] right_center_distance_cm right center (90 deg from front direction) distance in cm
     * @return right distance in cm, right distance is the smaller one of those two above
     */ 
    unsigned int get_side_distance_cm(long current_millis, unsigned long &right_front_distance_cm, unsigned long &right_center_distance_cm);
    /**
     * Measure front distance to wall or obstacle, also obtained measurement is sent out via serial interface
     * @param[in] current_millis current system tick
     * @return front distance in cm
     */ 
    unsigned int get_front_distance_cm(long current_millis);
    /**
     * Get all possible distance measurements in form of DistanceMeasurements structure, also obtained measurements
     * are sent out via serial interface
     * @param[in] current_millis current system tick
     * @param[out] distance_measurements structure holding the measurements
     */     
    void get_distance_measurements(long current_millis, DistanceMeasurements &distance_measurements); 
    /**
     * Disable sensing module, when disable all measurement requests will be ignored
     */     
    void disable()
    {
        disabled = true; 
    };

    /**
     * Enable sensing module, measurement requests will no longer be ignored
     */  
    void enable()
    {
        disabled = false;
    };

};

#endif // SENSING_H
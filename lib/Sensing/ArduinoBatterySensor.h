#ifndef ARDUINOBATTERYSENSOR_H
#define ARDUINOBATTERYSENSOR_H

#include <Arduino.h> // TODO: can we remove this from the header file and only include it in the cpp file?
#include "IBatterySensor.h"

/**
 * Class implementing battery voltage sensor for Arduino platform
 */
class ArduinoBatterySensor : public IBatterySensor
{
    static const uint8_t SNS_BATTERY_VLTG = A8;
    static const uint16_t ADC_REFERENCE_V = 5;
    static const uint16_t ADC_MAX = 1023;
public:
    /**
     * Constructor method
     */
    ArduinoBatterySensor();

    /**
     * Method to get battery voltage in ADC units
     * @return battery voltage in ADC units
     */
    uint16_t get_battery_voltage_adc() override;

    /**
     * Method to get battery voltage in volts
     * @return battery voltage in volts
     */
    uint16_t get_battery_voltage_V() override;
};

#endif // ARDUINOBATTERYSENSOR_H

#ifndef IBATTERYSENSOR_H
#define IBATTERYSENSOR_H

/**
 * Interface for battery voltage sensor
 */
class IBatterySensor
{
public:
    /**
     * Method to get battery voltage in ADC units
     * @return battery voltage in ADC units
     */
    virtual uint16_t get_battery_voltage_adc() = 0;

    /**
     * Method to get battery voltage in volts
     * @return battery voltage in volts
     */
    virtual uint16_t get_battery_voltage_VDC() = 0;

    virtual ~IBatterySensor() = default;
};

#endif // IBATTERYSENSOR_H

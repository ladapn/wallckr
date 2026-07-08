#ifndef ARDUINOBATTERYSENSOR_H
#define ARDUINOBATTERYSENSOR_H

#include "ArduinoBoardConfig.h"
#include "IBatterySensor.h"

/**
 * Class implementing battery voltage sensor for Arduino platform
 */
class ArduinoBatterySensor : public IBatterySensor {
  static const uint16_t ADC_REFERENCE_mV = 5000;
  static const uint16_t ADC_MAX = 1023;

public:
  /**
   * Method to get battery voltage in ADC units
   * @return battery voltage in ADC units
   */
  uint16_t get_battery_voltage_adc() override;

  /**
   * Method to get battery voltage in millivolts
   * @return battery voltage in millivolts
   */
  uint16_t get_battery_voltage_mVDC() override;
};

#endif // ARDUINOBATTERYSENSOR_H

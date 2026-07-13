#ifndef ARDUINOCURRENTSENSOR_H
#define ARDUINOCURRENTSENSOR_H

#include "ICurrentSensor.h"

/**
 * Class implementing the high-side current sensor for Arduino platform
 */
class ArduinoCurrentSensor : public ICurrentSensor {
public:
  /**
   * Method to get sensed current in ADC units
   * @return current reading in ADC units
   */
  uint16_t get_current_adc() override;
};

#endif // ARDUINOCURRENTSENSOR_H

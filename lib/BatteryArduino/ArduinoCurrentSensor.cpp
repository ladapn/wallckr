#include "ArduinoCurrentSensor.h"
#include "ArduinoBoardConfig.h"
#include <Arduino.h>

uint16_t ArduinoCurrentSensor::get_current_adc() {
  return analogRead(SNS_CURRENT_PIN);
}

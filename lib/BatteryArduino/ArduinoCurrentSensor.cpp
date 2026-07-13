#include "ArduinoCurrentSensor.h"
#include "ArduinoBoardConfig.h"
#include <Arduino.h>

namespace {
const uint16_t ADC_REFERENCE_mV = 5000;
const uint16_t ADC_MAX = 1023;

// High-side current-sense circuit component values (see README hardware
// list). I = (R_SENSE_OHM * R_LOAD_OHM / R_IN_OHM) * V_adc, where V_adc is
// the op-amp output read by the ADC. With these values the gain works out
// to exactly 1, i.e. 1 mV at the ADC pin corresponds to 1 mA sensed.
const float R_SENSE_OHM = 0.1f;
const float R_LOAD_OHM = 47000.0f;
const float R_IN_OHM = 4700.0f;
const float CURRENT_SENSE_GAIN = R_SENSE_OHM * R_LOAD_OHM / R_IN_OHM;
} // namespace

uint16_t ArduinoCurrentSensor::get_current_mA() {
  // static cast to uint32_t to avoid overflow during multiplication
  uint16_t v_adc_mV = static_cast<uint32_t>(analogRead(SNS_CURRENT_PIN)) *
                      ADC_REFERENCE_mV / ADC_MAX;
  return static_cast<uint16_t>(v_adc_mV * CURRENT_SENSE_GAIN);
}

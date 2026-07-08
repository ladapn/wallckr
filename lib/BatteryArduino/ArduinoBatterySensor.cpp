/* SPDX-License-Identifier: MIT */

#include "ArduinoBatterySensor.h"
#include <Arduino.h>

uint16_t ArduinoBatterySensor::get_battery_voltage_adc() {
  return analogRead(SNS_BATTERY_VLTG_PIN);
}

uint16_t ArduinoBatterySensor::get_battery_voltage_mVDC() {
  // static cast to uint32_t to avoid overflow during multiplication
  return static_cast<uint32_t>(analogRead(SNS_BATTERY_VLTG_PIN)) *
         ADC_REFERENCE_mV / ADC_MAX;
}

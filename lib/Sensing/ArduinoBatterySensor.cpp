/* SPDX-License-Identifier: MIT */

#include "ArduinoBatterySensor.h"
#include <Arduino.h>

unsigned int ArduinoBatterySensor::get_battery_voltage_adc() {
  return analogRead(SNS_BATTERY_VLTG);
}

unsigned int ArduinoBatterySensor::get_battery_voltage_mVDC() {
  return analogRead(SNS_BATTERY_VLTG) * ADC_REFERENCE_mV / ADC_MAX;
}

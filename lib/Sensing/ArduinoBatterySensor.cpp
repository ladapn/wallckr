/* SPDX-License-Identifier: MIT */

#include <Arduino.h>
#include "ArduinoBatterySensor.h"


unsigned int ArduinoBatterySensor::get_battery_voltage_adc()
{
    return analogRead(SNS_BATTERY_VLTG);
}

unsigned int ArduinoBatterySensor::get_battery_voltage_V()
{
    return analogRead(SNS_BATTERY_VLTG) * ADC_REFERENCE_V / ADC_MAX;
}

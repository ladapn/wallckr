#ifndef ARDUINO_ULTRASOUND_SENSOR_H
#define ARDUINO_ULTRASOUND_SENSOR_H

#include "ExpFilter.h"
#include "IDistanceSensor.h"
#include <NewPing.h>

constexpr int MAX_DISTANCE_CM = 200;

/**
 * Class representing ultrasound distance sensor, implements IDistanceSensor
 */
class ArduinoUltraSoundSensor : public IDistanceSensor {
  unsigned long m_measurement_raw_cm = 0;
  unsigned long m_measurement_filtered_cm = 0;
  unsigned int m_max_cm_distance;

  NewPing m_sonar;
  // TODO Would be better to accept reference to base class of filter as
  // constructor input
  static constexpr int FILTER_N = 4;
  ExpFilter<int, FILTER_N> m_sonar_filter;

public:
  ArduinoUltraSoundSensor(uint8_t trigger_pin, uint8_t echo_pin,
                          unsigned int max_cm_distance)
      : m_max_cm_distance(max_cm_distance),
        m_sonar(trigger_pin, echo_pin, max_cm_distance) {};

  /**
   * Command sensor to measure distance
   * @return always true, reserved for future use
   */
  bool trigger_measurement() override;

  /**
   * Command sensor to measure distance and return unfiltered measurement
   * @return unfiltered distance measurement in cm
   */
  unsigned long get_distance_raw_cm() override;

  /**
   * Command sensor to measure distance and return filtered measurement
   * @return filtered distance measurement in cm
   */
  unsigned long get_distance_filtered_cm() override;
};

#endif

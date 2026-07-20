#include "UltraSoundSensor.h"

bool UltraSoundSensor::trigger_measurement() {
  m_measurement_raw_cm = m_sonar.ping_cm();

  // Zero means out of range -> change the library, so I can distinguish
  // actual zero and out of range?
  if (m_measurement_raw_cm == 0) {
    m_measurement_raw_cm = m_max_cm_distance;
  }

  return true;
}

unsigned long UltraSoundSensor::get_distance_raw_cm() {
  trigger_measurement();

  return m_measurement_raw_cm;
}

unsigned long UltraSoundSensor::get_distance_filtered_cm() {
  trigger_measurement();

  m_measurement_filtered_cm = m_sonar_filter.next(m_measurement_raw_cm);

  return m_measurement_filtered_cm;
}

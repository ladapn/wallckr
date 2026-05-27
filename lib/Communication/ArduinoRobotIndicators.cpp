#include "ArduinoRobotIndicators.h"
#include <Arduino.h>

ArduinoRobotIndicators::ArduinoRobotIndicators() {
  // Initialize all LED pins as outputs
  pinMode(static_cast<unsigned int>(LEDPin::LED1), OUTPUT);
  pinMode(static_cast<unsigned int>(LEDPin::LED2), OUTPUT);
  pinMode(static_cast<unsigned int>(LEDPin::LED3), OUTPUT);
  pinMode(static_cast<unsigned int>(LEDPin::LED4), OUTPUT);
  pinMode(static_cast<unsigned int>(LEDPin::LED5), OUTPUT);

  // Turn off all LEDs initially (HIGH = off for this configuration)
  digitalWrite(static_cast<unsigned int>(LEDPin::LED1), HIGH);
  digitalWrite(static_cast<unsigned int>(LEDPin::LED2), HIGH);
  digitalWrite(static_cast<unsigned int>(LEDPin::LED3), HIGH);
  digitalWrite(static_cast<unsigned int>(LEDPin::LED4), HIGH);
  digitalWrite(static_cast<unsigned int>(LEDPin::LED5), HIGH);
}

void ArduinoRobotIndicators::setLED(unsigned int pin, bool state) {
  if (!isValidLED(pin))
    return;
  digitalWrite(pin, state ? LOW : HIGH);
}

int ArduinoRobotIndicators::indicate_robot_state(RobotIndicatorState state) {
  switch (state) {
  case RobotIndicatorState::MOVING_AVOIDING:
    // Indicate avoiding state (LED1 off, LED2 on)
    setLED(static_cast<unsigned int>(LEDPin::LED1), false);
    setLED(static_cast<unsigned int>(LEDPin::LED2), true);
    break;
  case RobotIndicatorState::MOVING_FOLLOWING:
    // Indicate following state (LED1 on, LED2 off)
    setLED(static_cast<unsigned int>(LEDPin::LED1), true);
    setLED(static_cast<unsigned int>(LEDPin::LED2), false);
    break;
  case RobotIndicatorState::IDLE:
  case RobotIndicatorState::ERROR:
    // Indicate idle state (LED1 off, LED2 off)
    setLED(static_cast<unsigned int>(LEDPin::LED1), false);
    setLED(static_cast<unsigned int>(LEDPin::LED2), false);
    break;
  }

  return 0;
}

int ArduinoRobotIndicators::indicate_battery_warning() {
  // Toggle battery warning LED (LED5)
  toggleBatteryLED();
  return 0;
}

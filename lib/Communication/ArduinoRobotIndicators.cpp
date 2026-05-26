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

int ArduinoRobotIndicators::indicate_robot_state() {
  // TODO: Implement LED pattern for robot state indication
  // Currently returns success code
  return 0;
}

int ArduinoRobotIndicators::indicate_battery_warning() {
  // Toggle battery warning LED (LED5)
  toggleBatteryLED();
  return 0;
}

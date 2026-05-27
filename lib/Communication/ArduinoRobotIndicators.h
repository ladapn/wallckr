#ifndef ARDUINO_ROBOT_INDICATORS_H
#define ARDUINO_ROBOT_INDICATORS_H

#include "IRobotIndicators.h"

/**
 * Pin assignments for indicator LEDs (PA2 to PA6)
 */
enum class LEDPin : unsigned int {
  LED1 = 24,
  LED2 = 25,
  LED3 = 26,
  LED4 = 27,
  LED5 = 28
};

/**
 * Manages indicator LEDs on the robot
 * Controls battery warning LED and provides state indication via LED patterns
 */
class ArduinoRobotIndicators : public IRobotIndicators {
private:
  static constexpr LEDPin BATTERY_LED = LEDPin::LED5;
  static constexpr unsigned int MIN_LED_PIN =
      static_cast<unsigned int>(LEDPin::LED1);
  static constexpr unsigned int MAX_LED_PIN =
      static_cast<unsigned int>(LEDPin::LED5);

  bool battery_led_on = false;

  /**
   * Check if LED pin is within valid range
   * @param[in] pin LED pin to validate
   * @return true if pin is valid, false otherwise
   */
  bool isValidLED(unsigned int pin) const {
    return pin >= MIN_LED_PIN && pin <= MAX_LED_PIN;
  }

  /**
   * Set LED state (Arduino: 0 = on, 1 = off)
   * @param[in] pin LED pin (use LEDPin enum)
   * @param[in] state true to turn on, false to turn off
   */
  void setLED(unsigned int pin, bool state);

  /**
   * Toggle battery indicator LED state
   */
  void toggleBatteryLED() {
    battery_led_on = !battery_led_on;
    setLED(static_cast<unsigned int>(BATTERY_LED), battery_led_on);
  }

public:
  /**
   * Constructor - initializes LED pins
   */
  ArduinoRobotIndicators();

  int indicate_robot_state(RobotIndicatorState state) override;
  int indicate_battery_warning() override;
};

#endif // ARDUINO_ROBOT_INDICATORS_H

#include "ArduinoMotorController.h"
#include "MotionConstants.h"
#include <Arduino.h>

/**
 * Alias for pin states causing forward/backward motor run
 */
enum class MotorDirection { FORWARD = HIGH, BACKWARD = LOW };

ArduinoMotorController::ArduinoMotorController(int dir_pin, int pwm_control_pin,
                                               int brake_control_pin) {
  direction_pin = dir_pin;
  pwm_pin = pwm_control_pin;
  brake_pin = brake_control_pin;

  // Direction pin on channel A
  pinMode(direction_pin, OUTPUT);

  pinMode(brake_pin, OUTPUT);
  // Start braked rather than coasting; released once actively driven in
  // set_speed(). Also set explicitly here (not just relied upon via Motion's
  // constructor calling stop()) so the motor is never left coasting due to
  // construction order between ArduinoMotorController and Motion.
  digitalWrite(brake_pin, HIGH);

  // set prescaler for Timer 3 (pin 3) to 1 to get 31372.55 Hz
  // to get motor PWM from audible range
  TCCR3B = (TCCR3B & 0b11111000) | 0x01;
}

bool ArduinoMotorController::set_speed(int speed) {
  if ((speed > MAX_SPEED) || (speed < -MAX_SPEED)) {
    return false;
  }

  auto motorDirection = MotorDirection::FORWARD;

  if (speed < 0) {
    motorDirection = MotorDirection::BACKWARD;
    speed = abs(speed);
  }

  digitalWrite(brake_pin, LOW); // Release the brake to drive
  digitalWrite(direction_pin,
               static_cast<uint8_t>(motorDirection)); // Set motor direction
  analogWrite(pwm_pin, speed); // Set the speed of the motor

  return true;
}

void ArduinoMotorController::stop() {
  analogWrite(pwm_pin, 0);
  digitalWrite(brake_pin, HIGH);
}

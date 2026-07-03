#ifndef ARUDINO_BOARD_CONFIG_H
#define ARUDINO_BOARD_CONFIG_H

#include <Arduino.h>

// Pin definitions
// Motion control pins
constexpr int MOTOR_PWM_PIN = 3;
constexpr int MOTOR_DIRECTION_PIN = 12;
constexpr int SERVO_PWM_PIN = 47;
constexpr int MOTOR_BRAKE_PIN = 9;

// Ultrasonic sensor pins
constexpr int TRIGGER_PIN_FRONT = 32;
constexpr int ECHO_PIN_FRONT = 33;

constexpr int TRIGGER_PIN_RIGHT_FRONT = 34;
constexpr int ECHO_PIN_RIGHT_FRONT = 35;

constexpr int TRIGGER_PIN_RIGHT_CENTER = 36;
constexpr int ECHO_PIN_RIGHT_CENTER = 37;

// Battery voltage sensing pin
constexpr uint8_t SNS_BATTERY_VLTG_PIN = A8;

// Indicator LED pins (PA2 to PA6)
constexpr unsigned int LED1_PIN = 24;
constexpr unsigned int LED2_PIN = 25;
constexpr unsigned int LED3_PIN = 26;
constexpr unsigned int LED4_PIN = 27;
constexpr unsigned int LED5_PIN = 28;

#endif // ARUDINO_BOARD_CONFIG_H

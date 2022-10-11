#include <Arduino.h>
#include "Robot.h"

void setup()
{
  // Initialize serial communication representing wired UART over USB connection
  Serial.begin(115200);
  Serial.println("wallckr");

  // Initialize serial communication representing wireless Bluetooth Low Energy (BLE) connection
  // BLE module is connected to Serial3
  Serial3.begin(115200); // 9600 default

  // Issue AT command to set BLE module name to "wallckr"
  Serial3.write("AT NAMEwallckr\r\n");
}

void loop()
{

  RobotPrinter BLE_out(Serial3);
  UltraSoundSensor sonar_front(TRIGGER_PIN_FRONT, ECHO_PIN_FRONT, MAX_DISTANCE_CM);
  UltraSoundSensor sonar_right_front(TRIGGER_PIN_RIGHT_FRONT, ECHO_PIN_RIGHT_FRONT, MAX_DISTANCE_CM);
  UltraSoundSensor sonar_right_center(TRIGGER_PIN_RIGHT_CENTER, ECHO_PIN_RIGHT_CENTER, MAX_DISTANCE_CM);
  Sensing robot_sensing(sonar_front, sonar_right_front, sonar_right_center, BLE_out);

  Motion robot_motion;

  OvladackaParser ovladacka_parser;
  ExternalCommandDecoder external_command_decoder(Serial3, ovladacka_parser);

  const int FILTER_N = 4;
  ExpFilter<int> servo_cmd_filter = ExpFilter<int>(FILTER_N);

  const int K_GAIN = 5;
  const int RIGHT_DISTANCE_SETPOINT_CM = 25;
  const int INSENSITIV_CM = 2;
  Regulator_P<int> side_distance_regulator = Regulator_P<int>(K_GAIN, RIGHT_DISTANCE_SETPOINT_CM, INSENSITIV_CM);
  LEDBar ledbar;
  AutoSteering<int> wall_following_steering = AutoSteering<int>(RIGHT_DISTANCE_SETPOINT_CM, ledbar, side_distance_regulator, servo_cmd_filter);

  Robot robot(robot_sensing, robot_motion, wall_following_steering, external_command_decoder, ledbar);


  while (true)
  {
    robot.main_loop(millis());
  }
}

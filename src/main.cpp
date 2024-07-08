//#include <Arduino.h>
#include "Robot.h"
#include "DummySerialStream.h"

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/sys/printk.h>

#include "BLE/BLESetup.h"

//void setup()
//{
  // Initialize serial communication representing wired UART over USB connection
  //Serial.begin(115200);
  //Serial.println("wallckr");

  // Initialize serial communication representing wireless Bluetooth Low Energy (BLE) connection
  // BLE module is connected to Serial3
  //Serial3.begin(115200); // 9600 default

  // Issue AT command to set BLE module name to "wallckr"
  //Serial3.write("AT NAMEwallckr\r\n");
//}

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);


int main(void)
{
  LOG_INF("Entering main...");

  ble_setup();

  DummySerialStream dummy_serial_stream;
  RobotPrinter BLE_out(dummy_serial_stream);
  UltraSoundSensor sonar_front(TRIGGER_PIN_FRONT, ECHO_PIN_FRONT, MAX_DISTANCE_CM);
  UltraSoundSensor sonar_right_front(TRIGGER_PIN_RIGHT_FRONT, ECHO_PIN_RIGHT_FRONT, MAX_DISTANCE_CM);
  UltraSoundSensor sonar_right_center(TRIGGER_PIN_RIGHT_CENTER, ECHO_PIN_RIGHT_CENTER, MAX_DISTANCE_CM);
  Sensing robot_sensing(sonar_front, sonar_right_front, sonar_right_center, BLE_out);

  Motion robot_motion;

  OvladackaParser ovladacka_parser;
  ExternalCommandDecoder external_command_decoder(dummy_serial_stream, ovladacka_parser);

  const int FILTER_N = 4;
  ExpFilter<int> servo_cmd_filter = ExpFilter<int>(FILTER_N);

  const int K_GAIN = 5;
  const int RIGHT_DISTANCE_SETPOINT_CM = 25;
  const int INSENSITIV_CM = 2;
  Regulator_P<int> side_distance_regulator = Regulator_P<int>(K_GAIN, RIGHT_DISTANCE_SETPOINT_CM, INSENSITIV_CM);
  LEDBar ledbar;
  AutoSteering<int> wall_following_steering = AutoSteering<int>(RIGHT_DISTANCE_SETPOINT_CM, ledbar, side_distance_regulator, servo_cmd_filter);

  Robot robot(robot_sensing, robot_motion, wall_following_steering, external_command_decoder, ledbar);


  // Active waiting not allowed in native posix builds
  //while (true)
  //{
    //robot.main_loop(millis());
    robot.main_loop(0);
  //}

  LOG_INF("main ends here...");

  return 0;
}

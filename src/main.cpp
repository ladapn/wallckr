#include <Arduino.h>
#include "ExternalCommandDecoder.h"
#include "BLEPackets.h"
#include "Regulator.h"
#include "UltraSoundSensor.h"
#include "LEDBar.h"
#include "Motion.h"
#include "Sensing.h"
#include "TimeManager.h"
#include "AutoSteering.h"
#include "RobotCommand.h"
#include "OvladackaParser.h"

const int TRIGGER_PIN_FRONT = 32;
const int ECHO_PIN_FRONT = 33;

const int TRIGGER_PIN_RIGHT_FRONT = 34;
const int ECHO_PIN_RIGHT_FRONT = 35;

const int TRIGGER_PIN_RIGHT_CENTER = 36;
const int ECHO_PIN_RIGHT_CENTER = 37;

const int MAX_DISTANCE_CM = 200;

void setup()
{
  // Open Serial communication
  Serial.begin(115200);
  Serial.println("wallckr");

  // Initialize BLE UART
  Serial3.begin(115200); // 9600 default

  // Set BLE's name
  Serial3.write("AT NAMEwallckr\r\n");
}

void loop()
{
  const int FILTER_N = 4;
  ExpFilter<int> servo_cmd_filter(FILTER_N);

  const int K_GAIN = 5;
  const int RIGHT_DISTANCE_SETPOINT_CM = 25;
  const int INSENSITIV_CM = 2;
  Regulator_P<int> side_distance_regulator(K_GAIN, RIGHT_DISTANCE_SETPOINT_CM, INSENSITIV_CM);

  BLE_printer BLE_out(Serial3);
  UltraSoundSensor sonar_front(TRIGGER_PIN_FRONT, ECHO_PIN_FRONT, MAX_DISTANCE_CM);
  UltraSoundSensor sonar_right_front(TRIGGER_PIN_RIGHT_FRONT, ECHO_PIN_RIGHT_FRONT, MAX_DISTANCE_CM);
  UltraSoundSensor sonar_right_center(TRIGGER_PIN_RIGHT_CENTER, ECHO_PIN_RIGHT_CENTER, MAX_DISTANCE_CM);

  Motion robot_motion;
  Sensing robot_sensing(sonar_front, sonar_right_front, sonar_right_center, BLE_out);

  LEDBar ledbar;

  OvladackaParser ovladacka_parser;
  ExternalCommandDecoder external_command_decoder(Serial3, ovladacka_parser);

  TimeManager time_manager;
  AutoSteering wall_following_steering(RIGHT_DISTANCE_SETPOINT_CM, ledbar);

  RobotCommand robot_command;

  while (true)
  {
    external_command_decoder.check_external_command(robot_command);

    robot_motion.set_speed_and_angle(robot_command.desired_speed, robot_command.desired_servo_angle);

    auto currentMillis = millis();

    if (time_manager.isTimeForStatusCheck(currentMillis))
    {
      if (!robot_sensing.battery_voltage_ok(currentMillis))
      {
        robot_motion.disable();
        robot_sensing.disable();

        ledbar.toggleBatteryLED();
      }
    }

    if (time_manager.isTimeForAutomaticCommand(currentMillis))
    {
      DistanceMeasurements distance_measurements;
      robot_sensing.get_distance_measurements(currentMillis, distance_measurements);   

      int servo_cmd = servo_cmd_filter.next(side_distance_regulator.action(distance_measurements.right_distance_cm)) + SERVO_CENTER;

      if (robot_command.enable_automatic_operation)
      {
        robot_command.desired_servo_angle = wall_following_steering.get_steering_command(distance_measurements.front_distance_cm, distance_measurements.right_front_distance_cm, servo_cmd);
      }
    }
  }
}

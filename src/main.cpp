#include "ArduinoBatterySensor.h"
#include "ArduinoBoardConfig.h"
#include "ArduinoMotorController.h"
#include "ArduinoSerialStream.h"
#include "ArduinoSteeringServo.h"
#include "Robot.h"
#include "PollingRobotRunner.h"
#include "RobotPrinter.h"
#include <Arduino.h>

void setup() {
  // Initialize serial communication representing wired UART over USB connection
  Serial.begin(115200);
  Serial.println("wallckr");

  // Initialize serial communication representing wireless Bluetooth Low Energy
  // (BLE) connection BLE module is connected to Serial3
  Serial3.begin(115200); // 9600 default

  // Issue AT command to set BLE module name to "wallckr"
  Serial3.write("AT NAMEwallckr\r\n");
}

void loop() {

  ArduinoSerialStream arduino_serial_stream(Serial3);
  RobotPrinter BLE_out(arduino_serial_stream);
  UltraSoundSensor sonar_front(TRIGGER_PIN_FRONT, ECHO_PIN_FRONT,
                               MAX_DISTANCE_CM);
  UltraSoundSensor sonar_right_front(TRIGGER_PIN_RIGHT_FRONT,
                                     ECHO_PIN_RIGHT_FRONT, MAX_DISTANCE_CM);
  UltraSoundSensor sonar_right_center(TRIGGER_PIN_RIGHT_CENTER,
                                      ECHO_PIN_RIGHT_CENTER, MAX_DISTANCE_CM);
  DistanceSensors distance_sensors = {sonar_front, sonar_right_front,
                                      sonar_right_center};
  ArduinoBatterySensor battery_sensor;
  Sensing robot_sensing(distance_sensors, battery_sensor, BLE_out);

  ArduinoMotorController motor_controller(MOTOR_DIRECTION_PIN, MOTOR_PWM_PIN,
                                          MOTOR_BRAKE_PIN);
  ArduinoSteeringServo steering_servo(SERVO_PWM_PIN);
  Motion robot_motion(motor_controller, steering_servo);

  OvladackaParser ovladacka_parser;
  ExternalCommandDecoder external_command_decoder(arduino_serial_stream,
                                                  ovladacka_parser);

  constexpr int FILTER_N = 4;
  ExpFilter<int, FILTER_N> servo_cmd_filter;

  const int K_GAIN = 5;
  const int RIGHT_DISTANCE_SETPOINT_CM = 25;
  const int INSENSITIV_CM = 2;
  Regulator_P<int> side_distance_regulator =
      Regulator_P<int>(K_GAIN, RIGHT_DISTANCE_SETPOINT_CM, INSENSITIV_CM);
  LEDBar ledbar;
  AutoSteering<int> wall_following_steering = AutoSteering<int>(
      RIGHT_DISTANCE_SETPOINT_CM, side_distance_regulator, servo_cmd_filter);

  Robot robot(robot_sensing, robot_motion, wall_following_steering,
              external_command_decoder, ledbar);

  PollingRobotRunner robot_runner(robot);

  while (true) {
    robot_runner.run_once(millis());
  }
}

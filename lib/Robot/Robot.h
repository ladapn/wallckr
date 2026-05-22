#ifndef ROBOT_H
#define ROBOT_H

#include "AutoSteering.h"
#include "ExternalCommandDecoder.h"
#include "LEDBar.h"
#include "Motion.h"
#include "OvladackaParser.h"
#include "Regulator.h"
#include "RobotCommand.h"
#include "RobotPackets.h"
#include "Sensing.h"
#include "UltraSoundSensor.h"

/**
 * Class representing the robot. It encapsulates sensing, motion and automatic
 * operation
 */
class Robot {
  Sensing &robot_sensing;
  Motion &robot_motion;
  AutoSteering<int> &wall_following_steering;

  ExternalCommandDecoder &external_command_decoder;

  RobotCommand robot_command;

  LEDBar &ledbar;

public:
  /**
   * Constructor method
   * @param[in] sensing object representing robot's sensing ability
   * @param[in] motion object representing robot's motion ability
   * @param[in] steering object responsible for automatic steering
   * @param[in] command_decoder object to decode external commands from user
   * @param[in] bar object representing row of LEDs
   */
  Robot(Sensing &sensing, Motion &motion, AutoSteering<int> &steering,
        ExternalCommandDecoder &command_decoder, LEDBar &bar)
      : robot_sensing(sensing), robot_motion(motion),
        wall_following_steering(steering),
        external_command_decoder(command_decoder), ledbar(bar) {}

  /**
   * Evaluate robot status, currently just checks if battery voltage is or is
   * not under threshold. If it is robot is disabled and battery led is toggled.
   * @param[in] current_millis current system time in ms
   */
  void perform_status_check(unsigned long current_millis) {
    if (!robot_sensing.battery_voltage_ok(current_millis)) {
      robot_motion.disable();
      robot_sensing.disable();

      ledbar.toggleBatteryLED();
    }
  }

  /**
   * Perform automatic action - i.e. compute desired steering servo angle based
   * on sensor measurements
   * @param[in] current_millis current system time in ms
   */
  void perform_automatic_action(unsigned long current_millis) {
    // Always evaluate measurements and desired servo angle, even if automatic
    // operation is disabled, to be able to switch it on immediately when
    // needed.
    DistanceMeasurements distance_measurements;
    robot_sensing.get_distance_measurements(current_millis,
                                            distance_measurements);
    auto desired_servo_angle =
        wall_following_steering.get_steering_command(distance_measurements);

    // TODO: don't check the command, instead introduce robot state field that
    // is set to AUTOMATIC or MANUAL based on external command, and check it
    // here.
    if (robot_command.enable_automatic_operation) {
      robot_command.desired_servo_angle = desired_servo_angle;

      auto steering_state = wall_following_steering.get_steering_state();
      if (steering_state == SteeringState::AVOIDING) {
        ledbar.switchLEDoff(LED1);
        ledbar.switchLEDon(LED2);
      } else {
        ledbar.switchLEDon(LED1);
        ledbar.switchLEDoff(LED2);
      }
    }
  }

  /**
   * Check if there is a new external command from user and if so, convert it
   * into RobotCommand instance
   */
  void check_external_command() {
    external_command_decoder.check_external_command(robot_command);
  }

  void apply_motion_command() {
    robot_motion.set_speed_and_angle(robot_command.desired_speed,
                                     robot_command.desired_servo_angle);
  }
};

#endif

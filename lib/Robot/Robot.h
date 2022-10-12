#ifndef ROBOT_H
#define ROBOT_H

#include "ExternalCommandDecoder.h"
#include "RobotPackets.h"
#include "Regulator.h"
#include "UltraSoundSensor.h"
#include "LEDBar.h"
#include "Motion.h"
#include "Sensing.h"
#include "TimeManager.h"
#include "AutoSteering.h"
#include "RobotCommand.h"
#include "OvladackaParser.h"
#include "UltraSoundPins.h"

/**
 * Class representing the robot. It encapsulates sensing, motion and automatic operation
 */
class Robot
{   
    Sensing &robot_sensing;
    Motion &robot_motion;
    AutoSteering<int> &wall_following_steering;

    ExternalCommandDecoder &external_command_decoder;

    TimeManager time_manager;
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
    Robot(Sensing &sensing, Motion &motion, AutoSteering<int> &steering, ExternalCommandDecoder &command_decoder, LEDBar &bar) : robot_sensing(sensing), 
    robot_motion(motion), 
    wall_following_steering(steering),
    external_command_decoder(command_decoder),
    ledbar(bar)
    {
        
    }

    /**
     * Evaluate robot status, currently just checks if battery voltage is or is not under threshold. If it is
     * robot is disabled and battery led is toggled.
     * @param[in] current_millis current system time in ms
     */
    void perform_status_check(unsigned long current_millis)
    {
        if (time_manager.isTimeForStatusCheck(current_millis))
        {
            if (!robot_sensing.battery_voltage_ok(current_millis))
            {
                robot_motion.disable();
                robot_sensing.disable();

                ledbar.toggleBatteryLED();
            }
        }
    }

    /**
     * Perform automatic action - i.e. compute desired steering servo angle based on sensor measurements
     * @param[in] current_millis current system time in ms
     */
    void perform_automatic_action(unsigned long current_millis)
    {
        if (time_manager.isTimeForAutomaticCommand(current_millis))
        {
            DistanceMeasurements distance_measurements;
            robot_sensing.get_distance_measurements(current_millis, distance_measurements);   
            robot_command.desired_servo_angle = wall_following_steering.get_steering_command(distance_measurements, robot_command.enable_automatic_operation);
        }
    }

    /**
     * Check if there is a new external command from user and if so, convert it into RobotCommand instance
     */ 
    void check_external_command()
    {
        external_command_decoder.check_external_command(robot_command);
    }

    /**
     * Perform one step of robot's main loop. Namely, check external command, perform status check, automatic action and command robot's motion 
     * accordingly
     * @param[in] current_millis current system time in ms 
     */
    void main_loop(unsigned long current_millis)
    {
        check_external_command();
        robot_motion.set_speed_and_angle(robot_command.desired_speed, robot_command.desired_servo_angle);
        perform_status_check(current_millis);
        perform_automatic_action(current_millis);
    }
};

#endif
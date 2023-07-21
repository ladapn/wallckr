#ifndef MOTION_H
#define MOTION_H

#include "MotionConstants.h"
#include "SteeringServo.h"
#include "Motor.h"

// Pin definitions 
const int MOTOR_PWM_PIN   = 3;
const int MOTOR_DIRECTION_PIN   = 12;
const int SERVO_PWM_PIN = 47;
const int MOTOR_BRAKE_PIN = 9; 


/**
 * Class responsible for robot movement, namely for control of motor and steering servo
 */ 
class Motion
{
    int prev_speed = 0;
    int oldServo = SERVO_CENTER;
    SteeringServo steering_servo;
    Motor motor;
    bool disabled = false;

public:

    /**
     * Constructor method, configures pins attached to servo and motor controls, stops
     * motor as a safety measure
     */
    Motion() : motor(MOTOR_DIRECTION_PIN, MOTOR_PWM_PIN, MOTOR_BRAKE_PIN)
    {
        stop();
        steering_servo.init(SERVO_PWM_PIN);
    }

    /**
     * Set motor speed to desired value. If the desired speed is the same as in previous call of this method, or 
     * if Motion module is disabled, nothing happens
     * @param[in] speed desired speed, positive value means forward motion, negative value backward motion, in both
     * cases maximal value is 255
     * @return true if speed command is different to previous one and Motion is not disabled, false otherwise
     */
    bool set_speed(int speed);
    /**
     * Set steering servo to desired value. If the desired value is the same as in previous call of this method, or 
     * if Motion module is disabled, nothing happens
     * @param[in] angle desired angle in degrees from 0 to 180
     * @return true if servo command is different to previous one and Motion is not disabled, false otherwise
     */
    bool set_angle(int angle);
    /**
     * Stops motor
     */
    void stop() { motor.stop(); };
    /** 
     * Set motor speed and steering servo angle to desired values. This is equivalent to calling set_speed and SetAngle
     * in sequence.
     * @param[in] speed desired speed, positive value means forward motion, negative value backward motion, in both
     * cases maximal value is 255
     * @param[in] angle desired angle in degrees from 0 to 180
     * @return logic and of return values of internally called set_speed and SetAngle
     */ 
    bool set_speed_and_angle(int speed, int angle);
    /**
     * Disabled Motion module. Robot motor is stopped and any command to motor and servo gets ignored. 
     */
    void disable() 
    {
        stop();
        disabled = true; 
    }
    /**
     * Enables Motion module. Commands to motor and servo will be accepted.  
     */
    void enable() { disabled = false; }
};

#endif
#ifndef MOTION_H
#define MOTION_H

#include "MotionConstants.h"
#include <Arduino.h>
#include <Servo.h>

// Pin definitions 
const int PWM_A   = 3;
const int DIR_A   = 12;
const int SERVO_A = 47;
const int BRAKE_A = 9; 

/**
 * Alias for pin states causing forward/backward motor run
 */
enum class MotorDirection { FORWARD = HIGH, BACKWARD = LOW };

/**
 * Class responsible for robot movement, namely for control of motor and steering servo
 */ 
class Motion
{
    int prev_speed = 0;
    int oldServo = SERVO_CENTER;
    Servo steering_servo;
    bool disabled = false;

public:

    /**
     * Constructor method, configures pins attached to servo and motor controls, stops
     * motor as a safety measure
     */
    Motion()
    {
        Stop();
        steering_servo.attach(SERVO_A);

        // Direction pin on channel A
        pinMode(DIR_A, OUTPUT);
        
        // Brake pin on channel A
        pinMode(BRAKE_A, OUTPUT);  

        // set prescaler for Timer 3 (pin 3) to 1 to get 31372.55 Hz
        // to get motor PWM from audible range 
        TCCR3B = (TCCR3B & 0b11111000) | 0x01;  
    }

    /**
     * Set motor speed to desired value. If the desired speed is the same as in previous call of this method, or 
     * if Motion module is disabled, nothing happens
     * @param[in] speed desired speed, positive value means forward motion, negative value backward motion, in both
     * cases maximal value is 255
     * @return true if speed command is different to previous one and Motion is not disabled, false otherwise
     */
    bool setSpeed(int speed);
    /**
     * Set steering servo to desired value. If the desired value is the same as in previous call of this method, or 
     * if Motion module is disabled, nothing happens
     * @param[in] angle desired angle in degrees from 0 to 180
     * @return true if servo command is different to previous one and Motion is not disabled, false otherwise
     */
    bool setAngle(int angle);
    /**
     * Stops motor
     */
    void Stop() { analogWrite(PWM_A, 0); }
    /**
     * Set motor speed and steering servo angle to desired values. This is equivalent to calling setSpeed and SetAngle
     * in sequence.
     * @param[in] speed desired speed, positive value means forward motion, negative value backward motion, in both
     * cases maximal value is 255
     * @param[in] angle desired angle in degrees from 0 to 180
     * @return logic and of return values of internally called setSpeed and SetAngle
     */ 
    bool set_speed_and_angle(int speed, int angle);
    /**
     * Disabled Motion module. Robot motor is stopped and any command to motor and servo gets ignored. 
     */
    void disable() 
    {
        Stop();
        disabled = true; 
    }
    /**
     * Enables Motion module. Commands to motor and servo will be accepted.  
     */
    void enable() { disabled = false; }
};

#endif
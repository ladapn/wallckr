#ifndef MOTION_H
#define MOTION_H

#include <Arduino.h>
#include <Servo.h>
#include "BLEJoystickDecoder.h"


// Pin definitions 
const int PWM_A   = 3;
const int DIR_A   = 12;
const int SERVO_A = 47;

// Servo motion definitions 
const int SERVO_INCREMENT = 10;
const int SERVO_CENTER = 80;
const int SERVO_MIN_RIGHT = 20;
const int SERVO_MAX_LEFT = 160;

// Motor definitions 
const int MAX_SPD = 255;
const int SPD_INCREMENT = 50;

class Motion
{
    int m_prev_speed;
    int m_oldServo;
    Servo m_steering_servo;

public:

    Motion() : m_prev_speed(0), m_oldServo(SERVO_CENTER)
    {
        m_steering_servo.attach(SERVO_A);

        // Direction pin on channel A
        pinMode(DIR_A, OUTPUT);  
    }

    bool setSpeed(int spd);
    bool setAngle(int angle);
    bool set_speed_and_angle(int spd, int angle);
    bool command(command_t cmd, int &spd, int &angle);
};

#endif
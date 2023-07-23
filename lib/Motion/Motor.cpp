#include <Arduino.h>
#include "Motor.h"
#include "MotionConstants.h"

/**
 * Alias for pin states causing forward/backward motor run
 */
enum class MotorDirection { FORWARD = HIGH, BACKWARD = LOW };

Motor::Motor(int dir_pin, int pwm_control_pin, int brake_control_pin)
{
    direction_pin = dir_pin;
    pwm_pin = pwm_control_pin;
    brake_pin = brake_control_pin;

    // Direction pin on channel A
    pinMode(direction_pin, OUTPUT);

    // TODO: break is currently unused 
    pinMode(brake_pin, OUTPUT);
        
    // set prescaler for Timer 3 (pin 3) to 1 to get 31372.55 Hz
    // to get motor PWM from audible range 
    TCCR3B = (TCCR3B & 0b11111000) | 0x01;  

}
    
bool Motor::set_speed(int speed)
{
    if ((speed > MAX_SPEED) || (speed < -MAX_SPEED))
    {
        return false;
    }

    auto motorDirection = MotorDirection::FORWARD;

    if (speed < 0)
    {
        motorDirection = MotorDirection::BACKWARD;
        speed = abs(speed);
    }

    digitalWrite(direction_pin, static_cast<uint8_t>(motorDirection)); // Set motor direction
    analogWrite(pwm_pin, speed);                                       // Set the speed of the motor

    return true;
}

void Motor::stop()
{
    analogWrite(pwm_pin, 0);
}
#ifndef MOTOR_H
#define MOTOR_H

class Motor
{
    int pwm_pin;
    int direction_pin;
    int brake_pin;
public:
    Motor(int dir_pin, int pwm_control_pin, int break_control_pin);
    bool set_speed(int speed);
    void stop();
};

#endif // MOTOR_H
#ifndef ROBOT_COMMAND_H
#define ROBOT_COMMAND_H

#include "MotionConstants.h"

struct RobotCommand
{
    int desired_speed = 0;
    int desired_servo_angle = SERVO_CENTER;
    bool enable_automatic_operation = true; 

};

#endif 
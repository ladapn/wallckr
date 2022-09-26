#ifndef AUTOSTEERING_H
#define AUTOSTEERING_H
#include "LEDBar.h"
#include "MotionConstants.h"
#include "Regulator.h"

class AutoSteering
{     
    const int TURNING_RADIUS_CM = 15;   
    unsigned int AVOIDING_DISTANCE_THR_CM;

    const int AVOIDING_DISTANCE_HYSTERESIS_CM = 20;

    enum class SteeringState {AVOIDING = 0, FOLLOWING};
    SteeringState automatic_state = SteeringState::FOLLOWING;
    LEDBar &ledbar;

public:
    AutoSteering(int right_distance_setpoint_cm, LEDBar &ldbar) : ledbar(ldbar)
    {
        // Distance to front obstacle that triggers turning, to end up with desired
        // side distance after the turn, this has to consist of the distance setpoint
        // and turning radius
        AVOIDING_DISTANCE_THR_CM = right_distance_setpoint_cm + TURNING_RADIUS_CM; 
    };

    int get_steering_command(unsigned long front_distance_cm, unsigned long right_front_distance_cm, int servo_action)
    {
        int desired_servo = servo_action;

        switch(automatic_state)
        {
            case SteeringState::FOLLOWING:
            
            if (front_distance_cm < AVOIDING_DISTANCE_THR_CM || right_front_distance_cm < 15) // Fixme magic constatn 
            {
                automatic_state = SteeringState::AVOIDING;
                // LED 1 on
                ledbar.switchLEDoff(LED1);
                ledbar.switchLEDon(LED2);                        
            }
            break;
            case SteeringState::AVOIDING:
            desired_servo = SERVO_MAX_LEFT;

            if (front_distance_cm > (AVOIDING_DISTANCE_THR_CM + AVOIDING_DISTANCE_HYSTERESIS_CM)
                && right_front_distance_cm > (AVOIDING_DISTANCE_THR_CM - 15)) // FIXME magic constant + filtering? 
            {        
                
                automatic_state = SteeringState::FOLLOWING;
                // LED 2 on
                ledbar.switchLEDon(LED1);
                ledbar.switchLEDoff(LED2);
                
            }          

            break;
        }

        return desired_servo; 
    }
};

#endif 
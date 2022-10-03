#ifndef AUTOSTEERING_H
#define AUTOSTEERING_H
#include "LEDBar.h"
#include "MotionConstants.h"
#include "Regulator.h"
#include "IFilter.h"

template <typename T>
class AutoSteering
{     
    const int TURNING_RADIUS_CM = 15;   
    unsigned int AVOIDING_DISTANCE_THR_CM;

    const int AVOIDING_DISTANCE_HYSTERESIS_CM = 20;

    enum class SteeringState {AVOIDING = 0, FOLLOWING};
    SteeringState automatic_state = SteeringState::FOLLOWING;
    LEDBar &ledbar;
    Regulator<T> &side_distance_regulator;
    IFilter<T> &servo_cmd_filter;

public:
    AutoSteering(int side_distance_setpoint_cm, LEDBar &ldbar, Regulator<T> &side_regulator, IFilter<T> &servo_flt) : ledbar(ldbar), 
    side_distance_regulator(side_regulator),
    servo_cmd_filter(servo_flt)
    {
        // Distance to front obstacle that triggers turning, to end up with desired
        // side distance after the turn, this has to consist of the distance setpoint
        // and turning radius
        AVOIDING_DISTANCE_THR_CM = side_distance_setpoint_cm + TURNING_RADIUS_CM; 
    };

    int get_steering_command(const DistanceMeasurements &distance_measurements, bool enable_automatic_operation)
    {
        auto desired_servo = servo_cmd_filter.next(side_distance_regulator.action(distance_measurements.right_distance_cm)) + SERVO_CENTER;
        
        if (!enable_automatic_operation)
        {
            return desired_servo;
        }        

        switch(automatic_state)
        {
            case SteeringState::FOLLOWING:
            
            if (distance_measurements.front_distance_cm < AVOIDING_DISTANCE_THR_CM || distance_measurements.right_front_distance_cm < 15) // Fixme magic constatn 
            {
                automatic_state = SteeringState::AVOIDING;
                // LED 1 on
                ledbar.switchLEDoff(LED1);
                ledbar.switchLEDon(LED2);                        
            }
            break;
            case SteeringState::AVOIDING:
            desired_servo = SERVO_MAX_LEFT;

            if (distance_measurements.front_distance_cm > (AVOIDING_DISTANCE_THR_CM + AVOIDING_DISTANCE_HYSTERESIS_CM)
                && distance_measurements.right_front_distance_cm > (AVOIDING_DISTANCE_THR_CM - 15)) // FIXME magic constant + filtering? 
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
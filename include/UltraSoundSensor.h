#ifndef ULTRASOUNT_SENSOR_H
#define ULTRASOUNT_SENSOR_H

#include <NewPing.h>
#include "CRegulator.h"

class UltraSoundSensor
{
    unsigned long m_measurement_raw_cm;
    unsigned long m_measurement_filtered_cm;
    unsigned int m_max_cm_distance;

    NewPing m_sonar;
    CExpFilter m_sonar_filter;

    public:

    UltraSoundSensor(uint8_t trigger_pin, uint8_t echo_pin, unsigned int max_cm_distance) :  m_max_cm_distance(max_cm_distance) ,
    m_sonar(trigger_pin, echo_pin, max_cm_distance)
    {};

    bool trigger_measuremen()
    {
        m_measurement_raw_cm = m_sonar.ping_cm();
        
        // Zero means out of range -> change the library, so I can distinguish actual zero and out of range?
        if(m_measurement_raw_cm == 0)  
        {
            m_measurement_raw_cm = m_max_cm_distance;
        }        

        return true;
    }
    
    unsigned long get_distance_raw_cm()
    {
        trigger_measuremen();

        return m_measurement_raw_cm;
    }
    
    unsigned long get_distance_filtered_cm()
    {
        trigger_measuremen();

        m_measurement_filtered_cm = m_sonar_filter.next_3_4(m_measurement_raw_cm);

        return m_measurement_filtered_cm;
    }
};

#endif
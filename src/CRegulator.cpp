#include "CRegulator.h"

float CRegulator::action(float in)
{
    float diff = m_setpoint - in;
    float out = 0;

    if (diff > m_insensitivity || diff < -m_insensitivity)
    {
        out = m_gain * diff;
    }
    
    if (out > m_action_limit)
    {
        out = m_action_limit;
    }
    else if (out < -m_action_limit)
    {
        out = -m_action_limit;
    }

    return out; 
}
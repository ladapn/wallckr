#include "CRegulator.h"

float CRegulator::action_P(float in)
{
    float e = m_setpoint - in;
    float out = 0;

    if (e > m_insensitivity || e < -m_insensitivity)
    {
        out = m_gain * e;  
    }
    
    if (out > m_action_limit_up)
    {
        out = m_action_limit_up;
    }
    else if (out < m_action_limit_bottom)
    {
        out = m_action_limit_bottom;
    }

    return out; 
}

float CRegulator::action_PD(float in)
{
    float e = m_setpoint - in;
    float out = 0;

    if(m_first_run)
    {
        m_last_e = e; 
        m_first_run = false; 
    }
    
    if (e > m_insensitivity || e < -m_insensitivity)
    {
        out = m_gain * e + (e - m_last_e) * m_D_gain;
    }
    
    if (out > m_action_limit_up)
    {
        out = m_action_limit_up;
    }
    else if (out < m_action_limit_bottom)
    {
        out = m_action_limit_bottom;
    }

    m_last_e = e;

    return out; 
}
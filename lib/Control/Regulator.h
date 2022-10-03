#ifndef REGULATOR_H
#define REGULATOR_H

#include "IFilter.h"

/** 
 * Regulator interface 
 */ 
template <typename T>
class Regulator
{
protected:
    T m_gain;

    T m_setpoint;
    T m_insensitivity;
    
    // TODO this should be also an input
    const T m_action_limit_up = 45;  
    const T m_action_limit_bottom = -30; 

    /**
     * Internal equation specific for each regulator type
     * @param[in] e difference of required and actual value
     * @return regulator action before limits are applied 
     */
    virtual T regulator_equation(T e) = 0;

public:
    /**
     * Constructor method
     * @param[in] gain gain used in regulator equation to multiply difference of required and actual value
     * @param[in] setpoint required value - the goal of the regulated quantity
     * @param[in] insen regulator insensitivity - if difference between required and actual value is lower
     * than insensitivity, it is considered zero
     */
    Regulator(T gain, T setpoint, T insen) : m_gain(gain), m_setpoint(setpoint), m_insensitivity(insen) {}; 
    /**
     * Perform regulator action
     * @param[in] in current value of regulated quantity
     * @return regulator action - input for regulated system in order to achieve required value on its output
     */
    virtual T action(T in)
    {        
        T e = this->m_setpoint - in;
        T out = 0;

        if (e > this->m_insensitivity || e < -this->m_insensitivity)
        {
            out = regulator_equation(e);  
        }
        
        if (out > this->m_action_limit_up)
        {
            out = this->m_action_limit_up;
        }
        else if (out < this->m_action_limit_bottom)
        {
            out = this->m_action_limit_bottom;
        }

        return out;    
    }
};

/**
 * Implementation of P (proportional) regulator
 */
template <typename T>
class Regulator_P : public Regulator<T>
{
protected:
    /**
     * Compute P regulator equation
     * @param[in] e difference of required and actual value
     * @return regulator action before limits are applied
     */
    T regulator_equation(T e) override
    {
        return this->m_gain * e;
    }

public:
    /**
     * Constructor method
     * @param[in] gain gain used in regulator equation to multiply difference of required and actual value
     * @param[in] setpoint required value - the goal of the regulated quantity
     * @param[in] insen regulator insensitivity - if difference between required and actual value is lower
     * than insensitivity, it is considered zero
     */
    Regulator_P(T gain, T setpoint, T insen) : Regulator<T>(gain, setpoint, insen) {}; 
    
}; 

/**
 * Implementation of PD (proportional derivative) regulator
 */
template <typename T>
class Regulator_PD : public Regulator<T>
{
protected:
    T m_D_gain;
    bool m_first_run; 
    T m_last_e;
    /**
     * Compute PD regulator equation
     * @param[in] e difference of required and actual value
     * @return regulator action before limits are applied
     */
    T regulator_equation(T e) override
    {
        if(m_first_run)
        {
            m_first_run = false;
            m_last_e = e; 
        }

        return this->m_gain * e + (e - m_last_e) * m_D_gain;
    }

public:
    /**
     * Constructor method
     * @param[in] gain gain used in regulator equation to multiply difference of required and actual value
     * @param[in] D_gain gain of derivative element
     * @param[in] setpoint required value - the goal of the regulated quantity
     * @param[in] insen regulator insensitivity - if difference between required and actual value is lower
     * than insensitivity, it is considered zero
     */
    Regulator_PD(T gain, T D_gain, T setpoint, T insen) : Regulator<T>(gain, setpoint, insen), m_D_gain(D_gain), m_first_run(true), m_last_e(0) {}; 
   
}; 





#endif

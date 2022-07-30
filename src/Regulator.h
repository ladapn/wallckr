// incl guard
#ifndef CREGULATOR_H
#define CREGULATOR_H


// Regulator interface 
template <typename T>
class Regulator
{
protected:
    T m_gain;

    T m_setpoint;
    T m_insensitivity;
    
    // FIXME this should be also an input
    const T m_action_limit_up = 45;  
    const T m_action_limit_bottom = -30; 

    virtual T regulator_equation(T e) = 0;

public:
    Regulator(T gain, T setpoint, T insen) : m_gain(gain), m_setpoint(setpoint), m_insensitivity(insen) {}; 
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

template <typename T>
class Regulator_P : public Regulator<T>
{
    protected:
    T regulator_equation(T e) override
    {
        return this->m_gain * e;
    }

    public:
    Regulator_P(T gain, T setpoint, T insen) : Regulator<T>(gain, setpoint, insen) {}; 
    
}; 

template <typename T>
class Regulator_PD : public Regulator<T>
{
    protected:
    T m_D_gain;
    bool m_first_run; 
    T m_last_e;

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
    Regulator_PD(T gain, T D_gain, T setpoint, T insen) : Regulator<T>(gain, setpoint, insen), m_D_gain(D_gain), m_first_run(true), m_last_e(0) {}; 
   
}; 


template <typename T> class ExpFilter
{
private:
    T m_state;
    bool m_first_step;
    const int m_N; 
    T core(T input);
public:
    ExpFilter(int N) : m_state(0), m_first_step(true), m_N(N) {};
    T next(T input)
    {
        if(m_first_step)
        {
            m_first_step = false;
            m_state = input;                  
        }
        else
        {
            m_state = core(input); 
        }
            
        return m_state;  
    }
};


#endif

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


const int atan2_1D_table[] = {0,2,3,5,7,8,10,11,13,14,16,17,19,20,22,23,25,26,27,28,30,31,32,33,34,36,37,38,39,40,41,42,42,43,44,45,46,47,47,48,49,50,50,51,51,52,53,53,54,54,55,56,56,57,57,58,58,58,59,59,60};
const int atan2_1D_table_max = sizeof(atan2_1D_table) / sizeof(atan2_1D_table[0]);

class CVectorDriver
{  
    private:
        const unsigned int scale = 8;
        const unsigned int MAX_FRONT_USND = 70; 
        const unsigned int right_usnd_setpoint = 35; // FIXME vstup konstruktoru
        
    public:
        int atan2_1D_lookup(int in)
        {
            int retval;

            if(in < 0)
            {
                in = in * -1; // FIXME better ABC
                // abs
                if(in > atan2_1D_table_max)
                {
                    retval = -atan2_1D_table[atan2_1D_table_max - 1];
                }
                else
                {
                    retval = -atan2_1D_table[in];
                }
                
                
            }
            if (in < atan2_1D_table_max)
            {
                retval = atan2_1D_table[in];
            }
            else
            {
                retval = atan2_1D_table[atan2_1D_table_max - 1];
            }            

            return retval;
            
        }

        int next_turning_delta(unsigned int front_usnd, unsigned int right_usnd) // FIXME jednotky do nazvu
        {
            // front inversion
            if (front_usnd < MAX_FRONT_USND)
            {
                // 70 - 50
                front_usnd = MAX_FRONT_USND - front_usnd;
                
                // 40 - 35 - (20 / 8)**2
                int vec_x = (right_usnd - right_usnd_setpoint) - (front_usnd / scale) * (front_usnd / scale);
                return atan2_1D_lookup(vec_x); 
            }
        }

        int next_servo_cmd(unsigned int front_usnd, unsigned int right_usnd)
        {
            int gain = 2; // FIXME
            return next_turning_delta(front_usnd, right_usnd) * gain;
        }

};

#endif

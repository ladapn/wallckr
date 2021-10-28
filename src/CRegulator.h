// incl guard
#ifndef CREGULATOR_H
#define CREGULATOR_H

// TODO: class should do only one thing -> class for P reg, PD reg... + interface
// template would be also usefull -> float/int... 
class CRegulator
{
    private:
        float m_gain;
        float m_D_gain;
        int m_setpoint;
        int m_insensitivity;
        float m_last_e; 
        bool m_first_run;
        int m_action_limit_up;
        int m_action_limit_bottom;
    public:
        CRegulator(float gain, int setpoint, int insen) : m_gain(gain), m_setpoint(setpoint), m_insensitivity(insen), m_last_e(0.0), m_first_run(true)
        {
            //TODO what is the maximal sensible limit? 
            m_action_limit_up = 45;
            m_action_limit_bottom = -30;

            //FIXME
            m_D_gain = 12.0;
        };
        void set_gain(float gain)
        {
            m_gain = gain;
        }

        float action_P(float in);
        int action_P(int in);
        float action_PD(float in);
};

// TODO: template? How to handle different computation for float and for int?
// also -> maybe a derived class for each filter type? 3/4, 1/2... -> or input parameter? 
class CExpFilter
{
    private:
        int m_state; //FIXME int ok?
        bool m_first_step; 
        //unsigned m_N; 
    public:
        CExpFilter() : m_state(0), m_first_step(true) {};
        //CExpFilter(unsigned N) : m_state(0), m_N(N) {};
        int next_3_4(int input)
        {
            if(m_first_step)
            {
                m_first_step = false;
                m_state = input;                  
            }
            else
            {                               
                //=BITRSHIFT(BITLSHIFT(F3, 2) - F3 + C5 + 2, 2)
                // 3/4 * state + 1/4 * input
                // (4*state - state + input + rounding_constatnt) / 4
                // WARNING: This never converges to zero! It gets stuck at 2
                m_state = ((m_state << 2) - m_state + input + 2) >> 2; 
            }
                
            return m_state;
            //return input;

        }
       
        // TODO virtual class and inheritance?
        int next_1_2(int input)
        {
            if(m_first_step)
            {
                m_first_step = false;
                m_state = input;                  
            }
            else
            {                               
                // This is na fact average
                m_state = (m_state  + input) >> 2; 
            }
                
            return m_state;
        }
       
};

class CExpFilterFlt
{
    private:
        float m_state;
        bool m_first_step; 
        //unsigned m_N; 
    public:
        CExpFilterFlt() : m_state(0.0), m_first_step(true) {};
        //CExpFilter(unsigned N) : m_state(0), m_N(N) {};
        int next_3_4(int input)
        {
            if(m_first_step)
            {
                m_first_step = false;
                m_state = input;                  
            }
            else
            {
                m_state = 0.75 * m_state + 0.25 * input; 
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

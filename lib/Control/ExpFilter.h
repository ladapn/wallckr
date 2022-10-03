#ifndef EXP_FILTER_H
#define EXP_FILTER_H

#include "IFilter.h"

template <typename T> class ExpFilter : public IFilter<T>
{
private:
    T m_state;
    bool m_first_step;
    const int m_N; 
    T core(T input);
public:
    explicit ExpFilter(int N) : m_state(0), m_first_step(true), m_N(N) {};
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
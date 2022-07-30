#include "CRegulator.h"


template<> int ExpFilter<int>::core(int input)
{
    if (m_N == 4)
    {
        //=BITRSHIFT(BITLSHIFT(F3, 2) - F3 + C5 + 2, 2)
        // 3/4 * state + 1/4 * input
        // (4*state - state + input + rounding_constatnt) / 4
        // WARNING: This never converges to zero! It gets stuck at 2
        return ((m_state << 2) - m_state + input + 2) >> 2; 
    }
    else
    {
        // TODO: Testing needed 
        return (((m_N - 1) * 100 / m_N) * m_state + 100/m_N * input) / 100; 
    }
}

template<> float ExpFilter<float>::core(float input)
{
    // 0.75 * m_state + 0.25 * input;
    if (m_N == 4)
    {
        return 0.75 * m_state + 0.25 * input;
    }
    else
    {
        return ((m_N - 1.0) / m_N) * m_state + 1.0/m_N * input;  
    }
}

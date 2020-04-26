// incl guard

class CRegulator
{
    private:
        float m_gain;
        int m_setpoint;
        int m_insensitivity; 
        int m_action_limit;
    public:
        CRegulator(float gain, int setpoint, int insen) : m_gain(gain), m_setpoint(setpoint), m_insensitivity(insen)
        {
            m_action_limit = 35;
        };
        void set_gain(float gain)
        {
            m_gain = gain;
        }

        float action(float in);
};


#ifndef TIME_MANAGER_H
#define TIME_MANAGER_H

class TimeManager 
{
    unsigned long lastStatusMillis = 0;
    unsigned long lastCommandMillis = 0;

    bool firstPassStatus = true;
    bool firstPassCommand = true; 

    static const int STATUS_PRINT_INTERVAL_MS = 1000;
    static const int COMMAND_INTERVAL_MS = 100;

public:
    bool isTimeForStatusCheck(unsigned long currentMillis)
    {
        if((currentMillis - lastStatusMillis > STATUS_PRINT_INTERVAL_MS) || firstPassStatus )
        {
            lastStatusMillis = currentMillis; 
            firstPassStatus = false;
            return true;
        }

        return false;         
    }

    bool isTimeForAutomaticCommand(unsigned long currentMillis)
    {
        if((currentMillis - lastCommandMillis > COMMAND_INTERVAL_MS) || firstPassCommand)
        {
            lastCommandMillis = currentMillis;
            firstPassCommand = false; 
            return true;
        }

        return false; 
    }
};

#endif // TIME_MANAGER_H
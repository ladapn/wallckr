#ifndef LED_BAR_H
#define LED_BAR_H

#include <Arduino.h>

enum leds {LED1 = 24, LED2 = 25, LED3 = 26, LED4 = 27, LED5 = 28};

class LEDBar
{
private:
    /* data */
public:
    LEDBar(/* args */);
    ~LEDBar();
    void switchLEDon(unsigned int LED)
    {
        // TODO get rid of these arduino calls 
        digitalWrite(LED, 0);
    }

    void switchLEDoff(unsigned int LED)
    {
        digitalWrite(LED, 1);
    }
};

LEDBar::LEDBar(/* args */)
{
    // TODO get rid of Arduino calls 
    pinMode(LED1, OUTPUT);
    pinMode(LED2, OUTPUT);
    pinMode(LED3, OUTPUT);
    pinMode(LED4, OUTPUT);
    pinMode(LED5, OUTPUT);

    digitalWrite(LED1, 1);
    digitalWrite(LED2, 1);
    digitalWrite(LED3, 1);
    digitalWrite(LED4, 1);
    digitalWrite(LED5, 1);
}

LEDBar::~LEDBar()
{
}


#endif
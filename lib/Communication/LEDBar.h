#ifndef LED_BAR_H
#define LED_BAR_H

//#include <Arduino.h>
/**
 * Definition of physical pin numbers connected to the LEDs
 * Pin groups PA2 to PA 6
 */
enum leds {LED1 = 24, LED2 = 25, LED3 = 26, LED4 = 27, LED5 = 28};

/**
 * Class representing row of LEDs present on robot
 */
class LEDBar
{
private:
    bool batteryLEDon = false;
    static const int BATTERY_LED = LED5;
public:
    LEDBar()
    {
        // TODO get rid of Arduino calls, since they are slow as hell
        /*pinMode(LED1, OUTPUT);
        pinMode(LED2, OUTPUT);
        pinMode(LED3, OUTPUT);
        pinMode(LED4, OUTPUT);
        pinMode(LED5, OUTPUT);*/

        // Default state - first LED on, others off
        /*digitalWrite(LED1, 0);
        digitalWrite(LED2, 1);
        digitalWrite(LED3, 1);
        digitalWrite(LED4, 1);
        digitalWrite(LED5, 1);*/
    }
    /**
     * Switch selected LED on
     * @param[in] LED pin (use leds enum)
     */
    void switchLEDon(unsigned int LED)
    {
        if(LED < LED1 || LED > LED5)
            return; 
        
        // TODO get rid of these arduino calls 
        //digitalWrite(LED, 0);
    }

    /**
     * Switch selected LED off
     * @param[in] LED pin (use leds enum)
     */
    void switchLEDoff(unsigned int LED)
    {
        if(LED < LED1 || LED > LED5)
            return; 
        
        //digitalWrite(LED, 1);
    }

    /**
     * Toggle state of LED dedicated to battery state (LED5)
     */    
    void toggleBatteryLED()
    {
        if (batteryLEDon)
        {
            batteryLEDon = false;
            switchLEDoff(BATTERY_LED);
        }
        else 
        {
            batteryLEDon = true;
            switchLEDon(BATTERY_LED);
        }
    }

};

#endif
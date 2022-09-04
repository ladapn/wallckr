#include "Sensing.h"
#include "BLEPackets.h"

bool Sensing::battery_voltage_ok(long currentMillis)
{
    auto battery_voltage_adc = analogRead(SNS_BATTERY_VLTG);

    sensor_printer.BLE_print_status_data(STATUS_ID, currentMillis, battery_voltage_adc);

    if(battery_voltage_adc <= BATTERY_PACK_CUTTOFF_ADC)
    {
        return false;        
    }    

    return true; 
}

unsigned int Sensing::get_front_distance_cm(long currentMillis)
{
    // Fire front sonar
    auto front_sonar_cm = sensor_front.get_distance_filtered_cm();

    sensor_printer.BLE_print_US_data(FRONT_US_ID, currentMillis, front_sonar_cm);

    return front_sonar_cm; 
}

unsigned int Sensing::get_side_distance_cm(long currentMillis, unsigned long &right_front_distance_cm, unsigned long &right_center_distance_cm)
{   

    // Fire right front sensor      
    right_front_distance_cm = sensor_right_front.get_distance_raw_cm(); 
    auto right_sonar_cm = right_front_distance_cm;

    sensor_printer.BLE_print_US_data(RIGHT_FRONT_US_ID, currentMillis, right_front_distance_cm);

    // Fire right center sensor 
    right_center_distance_cm = sensor_right_center.get_distance_raw_cm();

    if(right_center_distance_cm < right_sonar_cm)
    {
       right_sonar_cm = right_center_distance_cm;
    }

    sensor_printer.BLE_print_US_data(RIGHT_CENTER_US_ID, currentMillis, right_center_distance_cm);   

    return right_sonar_cm; 
}


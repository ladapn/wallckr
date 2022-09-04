#include "Sensing.h"

bool Sensing::battery_voltage_ok()
{
    auto battery_voltage_adc = analogRead(SNS_BATTERY_VLTG);

    if(battery_voltage_adc <= BATTERY_PACK_CUTTOFF_ADC)
    {
        return false;        
    } 

    return true; 
}

unsigned int Sensing::get_front_distance_cm()
{
    // Fire front sonar
    auto front_sonar_cm = sonar_front.get_distance_filtered_cm();

    //BLE_out.BLE_print_US_data(FRONT_US_ID, currentMillis, front_sonar_cm);

    return front_sonar_cm; 
}

unsigned int Sensing::get_side_distance_cm()
{
    

    // Fire right front sonar      
    auto right_front_sonar_cm = sonar_right_front.get_distance_raw_cm(); 
    auto right_sonar_cm = right_front_sonar_cm;

    //BLE_out.BLE_print_US_data(RIGHT_FRONT_US_ID, currentMillis, right_front_sonar_cm);

    // Fire right center sonar 
    auto right_center_sonar_cm = sonar_right_center.get_distance_raw_cm();

    if(right_center_sonar_cm < right_sonar_cm)
    {
       right_sonar_cm = right_center_sonar_cm;
    }

    //BLE_out.BLE_print_US_data(RIGHT_CENTER_US_ID, currentMillis, right_center_sonar_cm);   

    return right_sonar_cm; 
}


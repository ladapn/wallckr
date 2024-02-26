#include "Sensing.h"
#include "RobotPackets.h"

bool Sensing::battery_voltage_ok(long currentMillis)
{
    auto battery_voltage_adc = 0; //analogRead(SNS_BATTERY_VLTG);

    StatusPacket status_data(currentMillis, battery_voltage_adc, 0, 0);

    //sensor_printer.BLE_print_status_data(STATUS_ID, currentMillis, battery_voltage_adc);
    //sensor_printer.add_CRC_and_print(reinterpret_cast<uint8_t*>(&status_data), sizeof(status_data));

    if(battery_voltage_adc <= BATTERY_PACK_CUTTOFF_ADC)
    {
        return false;        
    }    

    return true; 
}

unsigned int Sensing::get_front_distance_cm(long currentMillis)
{
    if(disabled)
    {
        return 0; 
    }

    // Fire front sensor
    auto front_sonar_cm = sensor_front.get_distance_filtered_cm();

    SonarPacket sonar_packet(FRONT_US_ID, currentMillis, front_sonar_cm);

    //sensor_printer.BLE_print_US_data(FRONT_US_ID, currentMillis, front_sonar_cm);
    sensor_printer.add_CRC_and_print(reinterpret_cast<uint8_t*>(&sonar_packet), sizeof(sonar_packet));

    return front_sonar_cm; 
}

unsigned int Sensing::get_side_distance_cm(long currentMillis, unsigned long &right_front_distance_cm, unsigned long &right_center_distance_cm)
{   
    if(disabled)
    {
        return 0; 
    }

    // Fire right front sensor      
    right_front_distance_cm = sensor_right_front.get_distance_raw_cm(); 
    auto right_sonar_cm = right_front_distance_cm;

    SonarPacket sonar_packet(RIGHT_FRONT_US_ID, currentMillis, right_front_distance_cm);
    sensor_printer.add_CRC_and_print(reinterpret_cast<uint8_t*>(&sonar_packet), sizeof(sonar_packet));

    // Fire right center sensor 
    right_center_distance_cm = sensor_right_center.get_distance_raw_cm();

    if(right_center_distance_cm < right_sonar_cm)
    {
       right_sonar_cm = right_center_distance_cm;
    }

    sonar_packet.id = RIGHT_CENTER_US_ID;
    sonar_packet.tick = currentMillis;
    sonar_packet.sonar_data = right_center_distance_cm;
    sensor_printer.add_CRC_and_print(reinterpret_cast<uint8_t*>(&sonar_packet), sizeof(sonar_packet));   

    return right_sonar_cm; 
}

void Sensing::get_distance_measurements(long current_millis, DistanceMeasurements &distance_measurements)
{  

    distance_measurements.front_distance_cm = get_front_distance_cm(current_millis);
    distance_measurements.right_distance_cm = get_side_distance_cm(current_millis, 
                         distance_measurements.right_front_distance_cm,
                         distance_measurements.right_center_distance_cm);
}


#include <Arduino.h>
#include "BLEJoystickDecoder.h"
#include "BLEPackets.h"
#include "Regulator.h"
#include "UltraSoundSensor.h"
#include "LEDBar.h"
#include "Motion.h"
#include "Sensing.h"
#include "TimeManager.h"
#include "AutoSteering.h"

const int TRIGGER_PIN_FRONT = 32;  
const int ECHO_PIN_FRONT = 33; 

const int TRIGGER_PIN_RIGHT_FRONT = 34;
const int ECHO_PIN_RIGHT_FRONT = 35;

const int TRIGGER_PIN_RIGHT_CENTER = 36;
const int ECHO_PIN_RIGHT_CENTER = 37; 

const int MAX_DISTANCE_CM = 200;

void setup() {
  // Open Serial communication
  Serial.begin(115200);
  Serial.println("wallckr");

  // Itialize BLE UART
  Serial3.begin(115200); // 9600 default 

  // Set BLE's name
  Serial3.write("AT NAMEwallckr\r\n");

}

void loop() {
  int desiredSPD = 0;

  int desiredServo = SERVO_CENTER;
 
  const int FILTER_N = 4; 
  ExpFilter<int> servo_cmd_filter(FILTER_N);
  
  const int K_GAIN = 5;
  const int RIGHT_DISTANCE_SETPOINT_CM = 25;
  const int INSENSITIV_CM = 2;
  Regulator_P<int> side_distance_regulator(K_GAIN, RIGHT_DISTANCE_SETPOINT_CM, INSENSITIV_CM);

  BLE_printer BLE_out(Serial3); 
  UltraSoundSensor sonar_front(TRIGGER_PIN_FRONT, ECHO_PIN_FRONT, MAX_DISTANCE_CM);
  UltraSoundSensor sonar_right_front(TRIGGER_PIN_RIGHT_FRONT, ECHO_PIN_RIGHT_FRONT, MAX_DISTANCE_CM);
  UltraSoundSensor sonar_right_center(TRIGGER_PIN_RIGHT_CENTER, ECHO_PIN_RIGHT_CENTER, MAX_DISTANCE_CM);

  Motion robot_motion; 
  Sensing robot_sensing(sonar_front, sonar_right_front, sonar_right_center, BLE_out);

  LEDBar ledbar;
  ledbar.switchLEDon(LED1);
  ledbar.switchLEDoff(LED2);

  BLEJoystickDecoder external_command_decoder(Serial3); 

  TimeManager time_manager; 
  AutoSteering wall_following_steering(RIGHT_DISTANCE_SETPOINT_CM, ledbar); 

  while(true)
  {
    external_command_decoder.check_motion_cmd(desiredSPD, desiredServo);

    robot_motion.set_speed_and_angle(desiredSPD, desiredServo);
    
    auto currentMillis = millis();

    if(time_manager.isTimeForStatusCheck(currentMillis))
    {     
      if(!robot_sensing.battery_voltage_ok(currentMillis))
      {        
        robot_motion.disable();
        robot_sensing.disable();
                
        ledbar.toggleBatteryLED();        
      }
      // TODO: else -> enable stuff after some period of battery being ok      
    }

    if(time_manager.isTimeForAutomaticCommand(currentMillis))
    {                     
      unsigned long front_sonar_cm = robot_sensing.get_front_distance_cm(currentMillis); 
      unsigned long right_front_distance_cm, right_center_distance_cm;
      unsigned long right_sonar_cm = robot_sensing.get_side_distance_cm(currentMillis, right_front_distance_cm, right_center_distance_cm);
  
      int servo_cmd = servo_cmd_filter.next(side_distance_regulator.action(right_sonar_cm)) + SERVO_CENTER;

      // Driving state machine   
      desiredServo = wall_following_steering.get_steering_command(front_sonar_cm, right_front_distance_cm, servo_cmd);        
    }
  }
}

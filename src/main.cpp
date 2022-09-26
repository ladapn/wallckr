#include <Arduino.h>
#include "BLEJoystickDecoder.h"
#include "BLEPackets.h"
#include "Regulator.h"
#include "UltraSoundSensor.h"
#include "LEDBar.h"
#include "Motion.h"
#include "Sensing.h"
#include "TimeManager.h"

// TODO: when BLE signal lost for 5 sec, go to idle

const int TRIGGER_PIN_FRONT = 32;  
const int ECHO_PIN_FRONT = 33; 

const int TRIGGER_PIN_RIGHT_FRONT = 34;
const int ECHO_PIN_RIGHT_FRONT = 35;

const int TRIGGER_PIN_RIGHT_CENTER = 36;
const int ECHO_PIN_RIGHT_CENTER = 37; 

const int MAX_DISTANCE_CM = 200;

enum state_t {AVOIDING = 0, FOLLOWING};

const int BRAKE_A = 9;

const int K_GAIN = 5;
const int RIGHT_DISTANCE_SETPOINT_CM = 25;
const int INSENSITIV_CM = 2;

const int TURNING_RADIUS_CM = 15;

// Distance to front obstacle that triggers turning, to end up with desired
// side distance after the turn, this has to consist of the distance setpoint
// and turning radius 
const int AVOIDING_DISTANCE_THR_CM = RIGHT_DISTANCE_SETPOINT_CM + TURNING_RADIUS_CM;

const int AVOIDING_DISTANCE_HYSTERESIS_CM = 20; 

void setup() {
  // Configure the A output
  pinMode(BRAKE_A, OUTPUT);  // Brake pin on channel A

  // Open Serial communication
  Serial.begin(115200);
  Serial.println("wallckr");

  // set prescaler for Timer 3 (pin 3) to 1 to get 31372.55 Hz
  // TODO: what for?
  TCCR3B = (TCCR3B & 0b11111000) | 0x01;

  // Itialize BLE UART
  Serial3.begin(115200); // 9600 default 

  // Set BLE's name
  Serial3.write("AT NAMEwallckr\r\n");

}

void loop() {
  int desiredSPD = 0;

  int desiredServo = SERVO_CENTER;
 
  bool automatic_operation_en = true;
  
  state_t automatic_state = FOLLOWING;
  
  const int FILTER_N = 4; 
  ExpFilter<int> right_sonar_filter(FILTER_N);
  Regulator_P<int> side_distance_regulator(K_GAIN, RIGHT_DISTANCE_SETPOINT_CM, INSENSITIV_CM);

// robot_io? -> also contain ledbar
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

      // TODO: check impact on timming of other packets! Could this introduce too much delay to the control loop?      
    }

    if(time_manager.isTimeForAutomaticCommand(currentMillis))
    {
      //desiredServo = automatic_steering_control.get_command()
                     
      unsigned long front_sonar_cm = robot_sensing.get_front_distance_cm(currentMillis); 
      unsigned long right_front_distance_cm, right_center_distance_cm;
      unsigned long right_sonar_cm = robot_sensing.get_side_distance_cm(currentMillis, right_front_distance_cm, right_center_distance_cm);
  
      int servo_cmd = right_sonar_filter.next(side_distance_regulator.action(right_sonar_cm)) + SERVO_CENTER;

      // Driving state machine
     
      if(automatic_operation_en)
      {
        // desiredServo = steering_state_machine.get_command() -> nebo to cely dat do ty zakomentovany metody vyse? 
        switch(automatic_state)
        {
          case FOLLOWING:
            desiredServo = servo_cmd; 
            
            if (front_sonar_cm < AVOIDING_DISTANCE_THR_CM || right_front_distance_cm < 15) // Fixme magic constatn 
            {
              automatic_state = AVOIDING;
              // LED 1 on
              ledbar.switchLEDoff(LED1);
              ledbar.switchLEDon(LED2);                        
            }
          break;
          case AVOIDING:
            desiredServo = SERVO_MAX_LEFT;

            if (front_sonar_cm > (AVOIDING_DISTANCE_THR_CM + AVOIDING_DISTANCE_HYSTERESIS_CM)
                && right_front_distance_cm > (AVOIDING_DISTANCE_THR_CM - 15)) // FIXME magic constant + filtering? 
            {        
              
              automatic_state = FOLLOWING;
              // LED 2 on
              ledbar.switchLEDon(LED1);
              ledbar.switchLEDoff(LED2);
             
            }          

          break;
        }
      }  
      
    }
  }
}

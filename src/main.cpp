#include <Arduino.h>
#include <Servo.h>
#include <NewPing.h>
#include "BLEJoystickDecoder.h"
#include "BLEPackets.h"
#include "CRegulator.h"
#include "UltraSoundSensor.h"
#include "LEDBar.h"
#include "Motion.h"

// TODO: when BLE signal lost for 5 sec, go to idle

/*#define TRIGGER_PIN_FRONT  30  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN_FRONT     31  // Arduino pin tied to echo pin on the ultrasonic sensor.

#define TRIGGER_PIN_RIGHT_FRONT 32
#define ECHO_PIN_RIGHT_FRONT    33 

#define TRIGGER_PIN_RIGHT_CENTER  34
#define ECHO_PIN_RIGHT_CENTER     35 

#define TRIGGER_PIN_RIGHT_BACK  36
#define ECHO_PIN_RIGHT_BACK     37  */

#define TRIGGER_PIN_FRONT  32  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN_FRONT     33  // Arduino pin tied to echo pin on the ultrasonic sensor.

#define TRIGGER_PIN_RIGHT_FRONT 34
#define ECHO_PIN_RIGHT_FRONT    35 

#define TRIGGER_PIN_RIGHT_CENTER  36
#define ECHO_PIN_RIGHT_CENTER     37 

/*#define TRIGGER_PIN_RIGHT_BACK  36
#define ECHO_PIN_RIGHT_BACK     37  */


#define MAX_DISTANCE 200 

enum state_t {AVOIDING = 0, FOLLOWING};

const int BRAKE_A = 9;
//const int SNS_A   = A0;

const int COMMAND_INTERVAL_MS = 100;
const int STATUS_PRINT_INTERVAL_MS = 1000;
const int SNS_BATTERY_VLTG = A8;

// TODO: has to be in ADC units, 1 VDC - voltage drops between batteries and actual measurement point
// ALSO take into consideration the voltage divider 1:16
const int BATTERY_CELLS = 8;
// TODO: 1024 or 1023?
const int BATTERY_CUTTOFF_ADC = (0.9 / 5 * 1024 * BATTERY_CELLS) / 16;

const float K_GAIN = 5;
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
  Serial.println("Toy Car Demo:");

  // set prescaler for Timer 3 (pin 3) to 1 to get 31372.55 Hz
  // TODO: what for?
  TCCR3B = (TCCR3B & 0b11111000) | 0x01;

  // Itialize BLE UART
  Serial3.begin(115200); // 9600 default 

  // Set BLE's name
  Serial3.write("AT NAMERCcar\r\n");

}

void loop() {
  int desiredSPD = 0;

  int desiredServo = SERVO_CENTER;

  long int lastCommandMillis = 0;
  long int lastStatusMillis = 0;  
  long int currentMillis = 0;
  bool automatic_operation_en = true;

  bool battery_led_on = false; 
  
  state_t automatic_state = FOLLOWING;
  //int following_counter = 0;

  CExpFilter right_sonar_filter; //TODO: do I want a float here 

  CRegulator K_regulator(K_GAIN, RIGHT_DISTANCE_SETPOINT_CM, INSENSITIV_CM);

// robot_io? -> also contain ledbar
  BLE_printer BLE_out(Serial3); 
  UltraSoundSensor sonar_front(TRIGGER_PIN_FRONT, ECHO_PIN_FRONT, MAX_DISTANCE);
  UltraSoundSensor sonar_right_front(TRIGGER_PIN_RIGHT_FRONT, ECHO_PIN_RIGHT_FRONT, MAX_DISTANCE);
  UltraSoundSensor sonar_right_center(TRIGGER_PIN_RIGHT_CENTER, ECHO_PIN_RIGHT_CENTER, MAX_DISTANCE);
  //UltraSoundSensor sonar_right_back(TRIGGER_PIN_RIGHT_BACK, ECHO_PIN_RIGHT_BACK, MAX_DISTANCE);

  Motion robot_motion; 

  LEDBar ledbar;
  ledbar.switchLEDon(LED1);
  ledbar.switchLEDoff(LED2);

  bool first_pass = true; 

  BLEJoystickDecoder external_command_decoder(Serial3); 

  while(true)
  {
    external_command_decoder.check_motion_cmd(desiredSPD, desiredServo);

    robot_motion.set_speed_and_angle(desiredSPD, desiredServo);
    
    currentMillis = millis();

    if((currentMillis - lastStatusMillis > STATUS_PRINT_INTERVAL_MS) || first_pass)
    {
      first_pass = false; 
      lastStatusMillis = currentMillis; 
      // TODO: sensing.battery_check() -> if false returned, disable motion and sensing
      int battery_voltage_adc = analogRead(SNS_BATTERY_VLTG);

      if(battery_voltage_adc <= BATTERY_CUTTOFF_ADC)
      {        
        robot_motion.disable();
        // TODO: sensing.disable()

        //if cut of reached (~1 V per cell)
        if(battery_led_on)
        {
          ledbar.switchLEDoff(LED5);
          battery_led_on = false;
        }
        else
        {
          ledbar.switchLEDon(LED5);
          battery_led_on = true;            
        }
      }
      // TODO: else -> enable stuff after some period of battery being ok 

      // TODO: check impact on timming of other packets! Could this introduce too much delay to the control loop?        

      BLE_out.BLE_print_status_data(STATUS_ID, currentMillis, battery_voltage_adc);
    }

    if(currentMillis - lastCommandMillis > COMMAND_INTERVAL_MS)
    {
      //automatic_control.get_command()
      // -> side_distance = sensing.get_side_measurement()
      lastCommandMillis = currentMillis;
   
      // Fire front sonar
      unsigned long front_sonar_cm = sonar_front.get_distance_filtered_cm();

      BLE_out.BLE_print_US_data(FRONT_US_ID, currentMillis, front_sonar_cm);

      // Fire right front sonar      
      unsigned long right_front_sonar_cm = sonar_right_front.get_distance_raw_cm(); 
      unsigned long right_sonar_cm = right_front_sonar_cm;

      BLE_out.BLE_print_US_data(RIGHT_FRONT_US_ID, currentMillis, right_front_sonar_cm);

      // Fire right center sonar 
      unsigned long right_center_sonar_cm = sonar_right_center.get_distance_raw_cm();

      if(right_center_sonar_cm < right_sonar_cm)
      {
        right_sonar_cm = right_center_sonar_cm;
      }

      BLE_out.BLE_print_US_data(RIGHT_CENTER_US_ID, currentMillis, right_center_sonar_cm);

      // Fire right back sonar 
      /*unsigned long right_back_sonar_cm = sonar_right_back.get_distance_raw_cm();

      if(right_back_sonar_cm < right_sonar_cm)
      {
        right_sonar_cm = right_back_sonar_cm;
      }

      BLE_out.BLE_print_US_data(RIGHT_BACK_US_ID, currentMillis, right_back_sonar_cm);*/

      //float right_sonar_cm_flt = right_sonar_filter.next_3_4(right_sonar_cm);

      int servo_cmd = int(right_sonar_filter.next_3_4(K_regulator.action_P(float(right_sonar_cm)))) + SERVO_CENTER; 

      // Driving state machine
     
      if(automatic_operation_en)
      {
        switch(automatic_state)
        {
          case FOLLOWING:
            desiredServo = servo_cmd; 
            
            if (front_sonar_cm < AVOIDING_DISTANCE_THR_CM || right_front_sonar_cm < 15) // Fixme magic constatn 
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
                && right_front_sonar_cm > (AVOIDING_DISTANCE_THR_CM - 15)) // FIXME magic constant + filtering? 
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

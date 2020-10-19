#include <Arduino.h>
#include <Servo.h>
#include <NewPing.h>
#include <SharpIR.h>
#include "BLEJoystickDecoder.h"
#include "BLEPackets.h"
#include "CRegulator.h"

// TODO: when BLE signal lost for 5 sec, go to idle

#define TRIGGER_PIN_BACK  32
#define ECHO_PIN_BACK     33 

#define TRIGGER_PIN_RIGHT  34
#define ECHO_PIN_RIGHT     35 

#define TRIGGER_PIN  36  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     37  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 200 

typedef enum state {AVOIDING = 0, FOLLOWING} state_t;

const int PWM_A   = 3;
const int DIR_A   = 12;
const int BRAKE_A = 9;
//const int SNS_A   = A0;
const int SERVO_A = 47;

const int MAX_SPD = 255;  //200; //TBD
const int SPD_INCREMENT = 50;

const int SERVO_INCREMENT = 10;
const int SERVO_CENTER = 80;
const int SERVO_MIN_RIGHT = 20;
const int SERVO_MAX_LEFT = 160;

const int STATUS_PRINT_INTERVAL_MS = 100;

//const int SNS_BATTERY_VLTG = A14;

const float K_GAIN = 2;
const int SETPOINT_CM = 30;
const int INSENSITIV_CM = 5;

const int LED1 = 24; 
const int LED2 = 25; 

const int AVOIDING_DISTANCE_THR_CM = 50;

Servo myservo;  // create servo object to control a servo
SharpIR sensor(SharpIR::GP2Y0A21YK0F, A5);
SharpIR sensor2(SharpIR::GP2Y0A21YK0F, A4);

// TODO: class to encapsulate motor
bool motorCMD(int commandSPD)
{
  bool retval = false;
  bool motorDirection = HIGH; // HIGH -> forward

  if((commandSPD <= 255) && (commandSPD >= -255))
  {
    if(commandSPD < 0)
    {
      motorDirection = LOW;
      commandSPD = abs(commandSPD);
    }

    digitalWrite(DIR_A, motorDirection);    // Set motor direction
    analogWrite(PWM_A, commandSPD);     // Set the speed of the motor, 255 is the maximum value
    retval = true;
  }

  return retval;
}

void setup() {
  // Configure the A output
  pinMode(BRAKE_A, OUTPUT);  // Brake pin on channel A
  pinMode(DIR_A, OUTPUT);    // Direction pin on channel A

  // Open Serial communication
  Serial.begin(115200);
  Serial.println("Toy Car Demo:");

  myservo.attach(SERVO_A);

  // set prescaler for Timer 3 (pin 3) to 1 to get 31372.55 Hz
  TCCR3B = (TCCR3B & 0b11111000) | 0x01;

    // Itialize BLE UART
  Serial3.begin(115200); // 9600 default 

  // Set BLE's name
  Serial3.write("AT NAMERCcar\r\n");
  //Serial.write("Zadejte prikaz AT: \n\n");

//  analogReference(INTERNAL1V1);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);

  digitalWrite(LED2, 1);
  digitalWrite(LED1, 0);

}

void loop() {
  command_t incommingByte;
  int desiredSPD = 0;
  int oldSPD = 0;
  int desiredServo = SERVO_CENTER;
  int oldServo = SERVO_CENTER;
  long int lastMillis = 0;
  long int currentMillis = 0;
  bool automatic_operation_en = false;
  
  state_t automatic_state = FOLLOWING;
  int following_counter = 0;

  NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);
  NewPing sonar_right(TRIGGER_PIN_RIGHT, ECHO_PIN_RIGHT, MAX_DISTANCE);
  NewPing sonar_back(TRIGGER_PIN_BACK, ECHO_PIN_BACK, MAX_DISTANCE);

  CExpFilter front_sonar_filter;
  CExpFilter right_sonar_filter;
  CExpFilter back_sonar_filter;

  CRegulator K_regulator(K_GAIN, SETPOINT_CM, INSENSITIV_CM);

  int following_counter_max = 15;

  while(true)
  {
    incommingByte = command_decoder_fastest(Serial3); //Serial.read();
    if(incommingByte != NO_COMMAND)
    {
      switch(incommingByte)
      {
        case UP:
          if(desiredSPD < MAX_SPD)
          {
            desiredSPD += SPD_INCREMENT;

          }
        break;
        case DOWN:
          if(desiredSPD > -MAX_SPD)
          {
            desiredSPD -= SPD_INCREMENT;
          }
        break;
        case RIGHT:
          if(desiredServo > SERVO_MIN_RIGHT)
          {
            desiredServo -= SERVO_INCREMENT;
          }
        break;
        case LEFT:
          if(desiredServo < SERVO_MAX_LEFT)
          {
            desiredServo += SERVO_INCREMENT;
          }
        break;
        case UP_TRIANGLE:
          desiredServo = SERVO_CENTER;
        break;
        default:
          desiredSPD = 0; //Stop the engine
          //desiredServo = SERVO_CENTER;
          //myservo.write(45);
        //Serial.println(incommingByte);
      }
    }

    if(oldSPD != desiredSPD)
    {
      oldSPD = desiredSPD;
      motorCMD(desiredSPD);

      //Serial.println(desiredSPD);
    }

    if(oldServo != desiredServo)
    {
      oldServo = desiredServo;
      myservo.write(desiredServo);

      //Serial.println(desiredServo);
    }
    

    currentMillis = millis();
    if(currentMillis - lastMillis > STATUS_PRINT_INTERVAL_MS)
    {
      sonar_packet_t sp;
      lastMillis = currentMillis;
      //Serial3.println(analogRead(SNS_BATTERY_VLTG)); // * 5.0 / 1023.0);
      
      unsigned long front_sonar_cm = sonar.ping_cm();
       
      if(front_sonar_cm == 0) // Zero means out of range -> change the library, so I can distinguish actual zero and out of range? 
      {
        front_sonar_cm = MAX_DISTANCE;
      }

      front_sonar_cm = front_sonar_filter.next_3_4(front_sonar_cm);

      sp.id = 101;
      sp.sonar_data = front_sonar_cm; // sensor.getDistance(); //;
      sp.tick = currentMillis;
      sp.crc = 0;
      Serial3.write((uint8_t*)&sp, sizeof(sp));

      unsigned long right_sonar_cm = sonar_right.ping_cm();

      if(right_sonar_cm == 0) // Zero means out of range -> change the library, so I can distinguish actual zero and out of range? 
      {
        right_sonar_cm = MAX_DISTANCE;
      }

      right_sonar_cm = right_sonar_filter.next_3_4(right_sonar_cm);

      sp.id = 102;
      sp.sonar_data = right_sonar_cm; //sensor2.getDistance();//
      sp.tick = currentMillis;
      sp.crc = 0;
      Serial3.write((uint8_t*)&sp, sizeof(sp));

      // TODO Status packet!
     
      unsigned long back_sonar_cm = sonar_back.ping_cm();

      if(back_sonar_cm == 0) // Zero means out of range -> change the library, so I can distinguish actual zero and out of range? 
      {
        back_sonar_cm = MAX_DISTANCE;
      }

      sp.id = 103;
      sp.sonar_data = back_sonar_filter.next_3_4(back_sonar_cm);
      sp.tick = currentMillis;
      sp.crc = 0;
      Serial3.write((uint8_t*)&sp, sizeof(sp));

      if(automatic_operation_en)
      {
        switch(automatic_state)
        {
          case FOLLOWING:
            desiredServo = int(K_regulator.action(right_sonar_cm)) + SERVO_CENTER; 
            
            if (front_sonar_cm < AVOIDING_DISTANCE_THR_CM)
            {
              automatic_state = AVOIDING;
              // LED 1 on
              //FIXME -> get rid of arduino call
              digitalWrite(LED1, 1);
              digitalWrite(LED2, 0);
              
            }
          break;
          case AVOIDING:
            desiredServo = SERVO_MAX_LEFT;

            if (front_sonar_cm > (AVOIDING_DISTANCE_THR_CM + 20))
            {  
              if(following_counter < following_counter_max) 
              {
                following_counter++;
              }              
            }

            if(following_counter >= following_counter_max)
            {
              automatic_state = FOLLOWING;
              following_counter = 0;
              // LED 2 on
              //FIXME -> get rid of arduino call
              digitalWrite(LED1, 0);
              digitalWrite(LED2, 1);
            }          

          break;
        }
      }

      //Serial.println(analogRead(SNS_A));
      //TODO: print status - voltages, currents etc.
    }
  }
}

#include <Arduino.h>
#include <Servo.h>
#include <NewPing.h>
#include "BLEJoystickDecoder.h"
#include "BLEPackets.h"

// TODO: when BLE signal lost for 5 sec, go to idle

#define TRIGGER_PIN  36  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     37  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 200 

const int PWM_A   = 3;
const int DIR_A   = 12;
const int BRAKE_A = 9;
const int SNS_A   = A0;
const int SERVO_A = 47;

const int MAX_SPD = 255;  //200; //TBD
const int SPD_INCREMENT = 50;

const int SERVO_INCREMENT = 10;
const int SERVO_CENTER = 80;
const int SERVO_MIN_RIGHT = 20;
const int SERVO_MAX_LEFT = 160;

const int STATUS_PRINT_INTERVAL_MS = 1000;

const int SNS_BATTERY_VLTG = A14;

Servo myservo;  // create servo object to control a servo

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
  Serial3.begin(9600);

  // Set BLE's name
  Serial3.write("AT NAMERCcar\r\n");
  //Serial.write("Zadejte prikaz AT: \n\n");

//  analogReference(INTERNAL1V1);
}

void loop() {
  command_t incommingByte;
  int desiredSPD = 0;
  int oldSPD = 0;
  int desiredServo = SERVO_CENTER;
  int oldServo = SERVO_CENTER;
  long int lastMillis = 0;
  long int currentMillis = 0;
  
  NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

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
    }

    currentMillis = millis();
    if(currentMillis - lastMillis > STATUS_PRINT_INTERVAL_MS)
    {
      sonar_packet_t sp;
      lastMillis = currentMillis;
      //Serial3.println(analogRead(SNS_BATTERY_VLTG)); // * 5.0 / 1023.0);
      

      sp.id = 101;
      sp.sonar_data = sonar.ping_cm();
      sp.tick = currentMillis;
      sp.crc = 0;
      Serial3.write((uint8_t*)&sp, sizeof(sp));


      //Serial.println(analogRead(SNS_A));
      //TODO: print status - voltages, currents etc.
    }
  }
}


/*void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);

  // Itialize BLE UART
  Serial3.begin(9600);

  // Set BLE's name
  Serial3.write("AT NAMERCcar\r\n");
  //Serial.write("Zadejte prikaz AT: \n\n");

}

void loop() {
  // put your main code here, to run repeatedly:
  
  
  Serial.write(command_decoder(Serial3));
}*/
#include <Arduino.h>
#include "BLEJoystickDecoder.h"

void setup() {
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
}
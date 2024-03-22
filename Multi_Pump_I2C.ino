#include <Wire.h>
#include <Arduino.h>

#define SDA_PIN 21   // Green SCA
#define SCL_PIN 22   // White SCL
#define ADDRESS_PUMP1 109  
#define ADDRESS_PUMP2 110  

char command[20];             
byte commandLength = 0;       
bool commandReceived = false; 

char response[20];            
byte responseLength = 0;      

void setup() {
  Serial.begin(9600);
  Wire.begin(SDA_PIN, SCL_PIN);
}

void serialEvent() {
  commandLength = Serial.readBytesUntil('\n', command, sizeof(command)); // Read command until newline
  command[commandLength] = '\0'; // Null terminate the command
  commandReceived = true; // Set flag to indicate command received
}

void loop() {
  if (commandReceived) {
    if (strncmp(command, "P1,", 3) == 0) {
      controlPump(ADDRESS_PUMP1);
    } else if (strncmp(command, "P2,", 3) == 0) {
      controlPump(ADDRESS_PUMP2);
    } else {
      Serial.println("Invalid command format.");
    }
    commandReceived = false; // Reset flag
  }
}

void controlPump(int address) {
  Wire.beginTransmission(address); // Begin I2C communication with the pump
    
  // Send each character of the command individually after removing the prefix
  for (byte i = 3; i < commandLength; i++) {
    Wire.write(command[i]);
  }
  
  Wire.endTransmission(); // End I2C communication

  if (strcmp(command + 3, "sleep") != 0) {
    delay(250); // Delay to allow the pump to process the command

    Wire.requestFrom(address, sizeof(response)); // Request response from the pump
    responseLength = Wire.readBytesUntil('\0', response, sizeof(response)); // Read response until null character
    
    // Print response to serial monitor
    for (byte i = 0; i < responseLength; i++) {
      Serial.write(response[i]);
    }
    Serial.println(); // Newline after printing response
  }
}

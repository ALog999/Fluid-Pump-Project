#include <Wire.h>
#include <Arduino.h>

#define SDA_PIN 21   // Define the SDA pin
#define SCL_PIN 22   // Define the SCL pin
#define ADDRESS 109  // I2C address of the EZO-PMP-L pump

char command[20];             // Array to hold incoming command
byte commandLength = 0;       // Length of the command
bool commandReceived = false; // Flag to indicate if a command has been received

char response[20];            // Array to hold the response from the pump
byte responseLength = 0;      // Length of the response

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
    Wire.beginTransmission(ADDRESS); // Begin I2C communication with the pump
    
    // Send each character of the command individually
    for (byte i = 0; i < commandLength; i++) {
      Wire.write(command[i]);
    }
    
    Wire.endTransmission(); // End I2C communication

    if (strcmp(command, "sleep") != 0) {
      delay(250); // Delay to allow the pump to process the command

      Wire.requestFrom(ADDRESS, sizeof(response)); // Request response from the pump
      responseLength = Wire.readBytesUntil('\0', response, sizeof(response)); // Read response until null character
      
      // Print response to serial monitor
      for (byte i = 0; i < responseLength; i++) {
        Serial.write(response[i]);
      }
      Serial.println(); // Newline after printing response
    }

    commandReceived = false; // Reset flag
  }
}

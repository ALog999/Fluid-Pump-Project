#include <Wire.h>
#include <Arduino.h>

#define SDA_PIN 22   // Define the SDA pin
#define SCL_PIN 21   // Define the SCL pin
#define ADDRESS 109  // I2C address of the EZO-PMP-L pump

char command[] = "D,150\n";     // Command to send
byte commandLength = sizeof(command) - 1;  // Length of the command
bool commandReceived = true; // Flag to indicate command received

char response[20];            // Array to hold the response from the pump
byte responseLength = 0;      // Length of the response

void setup() {
  Serial.begin(9600);
  Wire.begin(SDA_PIN, SCL_PIN);
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

#include <Arduino.h>
#include <Wire.h>

#define I2C_SLAVE_ADDRESS 0x08

void receiveEvent(int bytesReceived);

void setup() {
  Serial.begin(9600);           // Start serial communication
  Wire.begin(I2C_SLAVE_ADDRESS); // Join I2C bus with slave address
  Wire.onReceive(receiveEvent); // Register receive event callback
  Serial.println("I2C Slave Ready");
}

void loop() {
  // Nothing needed here
}

// This function is called when data is received from the I2C master
void receiveEvent(int bytesReceived) {
  Serial.print("Received: ");
  while (Wire.available()) {
    char c = Wire.read(); // Receive byte
    Serial.print(c);      // Print character to Serial
  }
  Serial.println();
}

#include <Arduino.h>
#include <Wire.h>

#define I2C_ADDR 0x08  // Change this if needed
#define DEBUG_SERIAL Serial

// Test data buffer
uint8_t test_data[] = {12, 34, 56, 78}; 


// Called when STM32 writes data
void receiveEvent(int numBytes) {
  DEBUG_SERIAL.print("Received ");
  DEBUG_SERIAL.print(numBytes);
  DEBUG_SERIAL.println(" bytes:");

  while (Wire.available()) {
    uint8_t c = Wire.read();
    DEBUG_SERIAL.print("0x");
    DEBUG_SERIAL.print(c, HEX);
    DEBUG_SERIAL.print(" ");
  }
  DEBUG_SERIAL.println();
}

// Called when STM32 requests data
void requestEvent() {
  DEBUG_SERIAL.println("Master requested data. Sending test pattern.");
  Wire.write(test_data, sizeof(test_data));  // Send 4 bytes
}

void setup() {
  DEBUG_SERIAL.begin(115200);
  while (!DEBUG_SERIAL);  // Wait for serial port
  
  Wire.begin(I2C_ADDR);  // Join I2C bus as slave
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);
  
  DEBUG_SERIAL.println("Arduino I2C Slave Ready");
  DEBUG_SERIAL.print("Address: 0x");
  DEBUG_SERIAL.println(I2C_ADDR, HEX);
}

void loop() {
  delay(1000);  // Not strictly needed
}
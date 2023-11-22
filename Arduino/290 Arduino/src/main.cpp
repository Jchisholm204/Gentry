#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <ArduinoBLE.h>
#include "LSM6DS3.h"

BLEService Service("19B10000-E8F2-537E-4F6C-D104768A1214");
BLEFloatCharacteristic angleCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1214", BLERead);

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);

  BLE.begin();

  BLE.setLocalName("aaa");
  BLE.setAdvertisedService(Service);

  Service.addCharacteristic(angleCharacteristic);

  BLE.addService(Service);

  angleCharacteristic.writeValue(0);
  BLE.advertise();
  IMU.begin();
}

void loop() {
  BLEDevice central = BLE.central();
  if (central) {
    while (central.connected()) {
      float x, y, z;
      IMU.readAcceleration(x, y, z);
      Serial.println(x);
      angleCharacteristic.writeValue(x);
      if(angleCharacteristic.written()){
        if(angleCharacteristic.value()){
          digitalWrite(LED_BUILTIN, HIGH);
        }
        else{
          digitalWrite(LED_BUILTIN, LOW);
        }
      }
    }
  }
}
#include <Arduino.h>

#define but PA4
#define led PA3

void setup() {
  Serial.begin(9600);
}

void loop() {
  Serial.println(millis());
  delay(100);
}
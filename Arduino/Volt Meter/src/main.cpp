#include <Arduino.h>

void setup() {
  pinMode(A0, INPUT);
  Serial.begin(9600);
}

void loop() {
  Serial.println(((float)analogRead(A0))*5.0/1024.0);
  delay(100);
}
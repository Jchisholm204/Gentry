#include <Arduino.h>

#define but PA4
#define led PA3

void setup() {
  pinMode(led, OUTPUT);
  pinMode(but, INPUT);
  Serial.begin(9600);
}

void loop() {
  digitalWrite(led, digitalRead(but));
  Serial.println(digitalRead(but));
}
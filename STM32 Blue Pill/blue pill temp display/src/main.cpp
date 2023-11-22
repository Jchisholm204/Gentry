/**
 * @file main.cpp
 * @author Jacob Chisholm (jchisholm204.github.io)
 * @brief 4 Number 7 Segment Display temperature display
 * @version 1.0
 * @date 2023-01-13
 * 
 * @copyright Copyright (c) 2023
 * 
 * 
 * Made for STM Blue Pill
 */

#include <Arduino.h>
#include "Wire.h"
#include "Adafruit_BMP280.h"

Adafruit_BMP280 bmp;

// set up pins
//                    a   b   c   d   e   f   g   dt
uint8_t segments[] = {PB4, PB3, PA4, PA7, PA6, PB9, PB0, PA5};
//                    0   1  2   3
uint8_t num_pins[] = {PB12, PB13, PA15, PA12};

// FOR COMMON CATHODE
// ! FOR COMMON ANODE
bool numbers[10][8] = {
// a  b  c  d  e  f  g  dt
  {1, 1, 1, 1, 1, 1, 0, 0}, // 0
  {0, 1, 1, 0, 0, 0, 0, 0}, // 1
  {1, 1, 0, 1, 1, 0, 1, 0}, // 2
  {1, 1, 1, 1, 0, 0, 1, 0}, // 3
  {0, 1, 1, 0, 0, 1, 1, 0}, // 4
  {1, 0, 1, 1, 0, 1, 1, 0}, // 5
  {1, 0, 1, 1, 1, 1, 1, 0}, // 6
  {1, 1, 1, 0, 0, 0, 0, 0}, // 7
  {1, 1, 1, 1, 1, 1, 1, 0}, // 8
  {1, 1, 1, 0, 0, 1, 1, 0}  // 9
};

void setup() {
  for (int i = 0; i < 8; i++)
  {
    pinMode(segments[i], OUTPUT);
  }
  for (int i = 0; i < 4; i++)
  {
    pinMode(num_pins[i], OUTPUT);
  }

  Wire.begin();
  unsigned status = bmp.begin(0x76);
    /* Default settings from datasheet. */
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */
}

// FOR COMMON ANODE
void set(uint8_t segment, uint8_t number){
  switch (segment)
  {
  case 0:
    digitalWrite(num_pins[0], HIGH);
    digitalWrite(num_pins[1], LOW);
    digitalWrite(num_pins[2], LOW);
    digitalWrite(num_pins[3], LOW);
    for (int i = 0; i < 7; i++)
    {
      digitalWrite(segments[i], !numbers[number][i]);
      //Serial.printf("%d %d %d\n", i, segments[i], numbers[0][i]);
    }
    break;
    case 1:
    digitalWrite(num_pins[0], LOW);
    digitalWrite(num_pins[1], HIGH);
    digitalWrite(num_pins[2], LOW);
    digitalWrite(num_pins[3], LOW);
    for (int i = 0; i < 7; i++)
    {
      digitalWrite(segments[i], !numbers[number][i]);
    }
    break;
    case 2:
    digitalWrite(num_pins[0], LOW);
    digitalWrite(num_pins[1], LOW);
    digitalWrite(num_pins[2], HIGH);
    digitalWrite(num_pins[3], LOW);
    for (int i = 0; i < 7; i++)
    {
      digitalWrite(segments[i], !numbers[number][i]);
    }
    break;
    case 3:
    digitalWrite(num_pins[0], LOW);
    digitalWrite(num_pins[1], LOW);
    digitalWrite(num_pins[2], LOW);
    digitalWrite(num_pins[3], HIGH);
    for (int i = 0; i < 7; i++)
    {
      digitalWrite(segments[i], !numbers[number][i]);
    }
    break;
  
  default:
    break;
  }
  if (segment == 1){
    digitalWrite(PA5, LOW);
  }
  else{
    digitalWrite(PA5, HIGH);
  }
}

// overflow counters (for individual segments)
int a = 0;
int b = 0;
int c = 0;
int d = 0;

void loop() {
  set(0, a);
  delay(2); // 2_ms delay for switching between numbers
  set(1, b);
  delay(2); // 2_ms delay for switching between numbers
  set(2, c);
  delay(2); // 2_ms delay for switching between numbers
  set(3, d);
  delay(2); // 2_ms delay for switching between numbers

  int temp = bmp.readTemperature()*100;

  a = ((temp/1000) % 10);
  b = ((temp/100) % 10);
  c = ((temp/10) % 10);
  d = ((temp) % 10);

}
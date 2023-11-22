/**
 * @file main.cpp
 * @author Jacob Chisholm (jchisholm204.github.io)
 * @brief 4 Number 7 Segment Display Silly Frosh
 * @version 1.0
 * @date 2023-01-13
 * 
 * @copyright Copyright (c) 2023
 * 
 * 
 * Made for STM Blue Pill
 */

#include <Arduino.h>

// set up pins
//                    a   b   c   d   e   f   g   dt
uint8_t segments[] = {PB4, PB3, PA4, PA7, PA6, PB9, PB0, PA5};
//                    0   1  2   3
uint8_t num_pins[] = {PB12, PB13, PA15, PA12};


// FOR COMMON CATHODE
// ! FOR COMMON ANODE
bool numbers[16][8] = {
// a  b  c  d  e  f  g  dt
  {0, 0, 0, 0, 0, 0, 0, 0}, // space
  {0, 0, 0, 0, 0, 0, 0, 0}, // space
  {0, 0, 0, 0, 0, 0, 0, 0}, // space
  {1, 0, 1, 1, 0, 1, 1, 0}, // s
  {0, 0, 0, 0, 1, 1, 0, 0}, // i
  {0, 0, 0, 1, 1, 1, 0, 0}, // l
  {0, 0, 0, 1, 1, 1, 0, 0}, // l
  {0, 1, 1, 1, 0, 1, 1, 0}, // y
  {0, 0, 0, 0, 0, 0, 0, 0}, // space
  {1, 0, 0, 0, 1, 1, 1, 0}, // f
  {0, 0, 0, 0, 1, 0, 1, 0}, // r
  {1, 1, 1, 1, 1, 1, 0, 0}, // o
  {1, 0, 1, 1, 0, 1, 1, 0}, // s
  {0, 1, 1, 0, 1, 1, 1, 1}, // h
  {0, 0, 0, 0, 0, 0, 0, 0}, // space
  {0, 0, 0, 0, 0, 0, 0, 0}, // space
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
    for (int i = 0; i < 8; i++)
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
    for (int i = 0; i < 8; i++)
    {
      digitalWrite(segments[i], !numbers[number][i]);
    }
    break;
    case 2:
    digitalWrite(num_pins[0], LOW);
    digitalWrite(num_pins[1], LOW);
    digitalWrite(num_pins[2], HIGH);
    digitalWrite(num_pins[3], LOW);
    for (int i = 0; i < 8; i++)
    {
      digitalWrite(segments[i], !numbers[number][i]);
    }
    break;
    case 3:
    digitalWrite(num_pins[0], LOW);
    digitalWrite(num_pins[1], LOW);
    digitalWrite(num_pins[2], LOW);
    digitalWrite(num_pins[3], HIGH);
    for (int i = 0; i < 8; i++)
    {
      digitalWrite(segments[i], !numbers[number][i]);
    }
    break;
  
  default:
    break;
  }
}

uint8_t idx = 0;
uint16_t timer = 0;

void loop() {

  if(idx > 15) idx = 0;
  if(timer>1000) idx++, timer = 0;

  set(0, idx);
  delay(2); // 2_ms delay for switching between numbers
  set(1, idx+1);
  delay(2); // 2_ms delay for switching between numbers
  set(2, idx+2);
  delay(2); // 2_ms delay for switching between numbers
  set(3, idx+3);
  delay(2); // 2_ms delay for switching between numbers

  timer += 8;
}
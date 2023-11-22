/**
 * @file main.cpp
 * @author Jacob Chisholm (jchisholm204.github.io)
 * @brief 4 Number 7 Segment Display Counting Example
 * @version 1.0
 * @date 2023-01-06
 * 
 * @copyright Copyright (c) 2023
 * 
 * Increment a counter by 1 every 1000_ms.
 * Simple program that shows how to use a 4 number 7 segment display.
 * 
 * Made for ESP32-WROOM-32
 */

#include <Arduino.h>

// set up pins
//                    a   b   c   d   e   f   g   dt
uint8_t segments[] = {33, 25, 26, 12, 14, 32, 13, 27};
//                    0   1  2   3
uint8_t num_pins[] = {18, 5, 19, 21};


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

// overflow counters (for individual segments)
int a = 0;
int b = 0;
int c = 0;
int d = 0;
// loop timer
int timer = 0;

void loop() {
  set(0, a);
  delay(2); // 2_ms delay for switching between numbers
  set(1, b);
  delay(2); // 2_ms delay for switching between numbers
  set(2, c);
  delay(2); // 2_ms delay for switching between numbers
  set(3, d);
  delay(2); // 2_ms delay for switching between numbers
  timer +=8; // increment timer (2_ms * 4)

  if(timer > 1000){ // update counters every 1000_ms
    d++;
    if(d>9){
      c++;
      d=0;
    }
    if(c>9){
      b++;
      c=0;
    }
    if(b>9){
      a++;
      b=0;
    }
    timer = 0;
  }
}
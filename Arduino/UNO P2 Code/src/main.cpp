/**
 * @file main.cpp
 * @author Jacob Chisholm
 * @brief APSC 101 Example Code
 * @version 0.1
 * @date 2022-11-04
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#include <AFMotor.h>
#include <Arduino.h>
#include "arm.hpp"
#include "exp.hpp"

//    Ports:
// Motor Sheild Ports
#define Port_Motor_Stepper 2
#define Port_Motor_DC 1
// Joystick Analog Input Ports
#define Port_Joy_y 0
#define Port_Joy_x 0
#define Port_Joy_b 0
// FSR Port
#define Port_FSR 0

// Connect a stepper motor with 2048 steps per revolution for the Gripper
AF_Stepper gripper(2048, Port_Motor_Stepper);
// Connect a DC Motor for the Arm
AF_DCMotor arm(Port_Motor_DC);

fsr FSR(Port_FSR);

void setup() {
  // Joystick Inputs
  pinMode(Port_Joy_x, INPUT); 
  pinMode(Port_Joy_y, INPUT);
  pinMode(Port_Joy_b, INPUT);

  // Set up serial communication to print things to the serial monitor at 9600 bps
  // and print an initial statement
  Serial.begin(9600);
  Serial.println("Initialize..");
  Serial.print("Initial FSR Reading: ");
  Serial.print(FSR.getReading());

  // Set initial Stepper Motor Speed
  gripper.setSpeed(20);
}

void loop() {

  double armSpeed = exponential(analogRead(Port_Joy_y), 4.5, 0.1/*joydead*/, 0.05);
  arm.setSpeed(armSpeed);

  if(FSR.getState()){
    gripper.setSpeed(0);
  }
  else{
    gripper.setSpeed(20);
  }

  // this may be with wrong values I dont remember how the internals of the joysticks work
  if(analogRead(Port_Joy_x) > 0.1){
    gripper.onestep(FORWARD, MICROSTEP);
  }
  else if(analogRead(Port_Joy_x) < -0.1){
    gripper.onestep(BACKWARD, MICROSTEP);
  }

  delay(20);
}

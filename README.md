# Gentry
#### Embedded C Projects by Jchisholm204

This repository serves as a monolithic repository to contain all of my embedded projects built under various architectures and abstraction layers.

Anyone is free to use any of the code contained within this repository so long as the following conditions are followed:
- Credit is given to the original creator
- Not sold without direct permission from the creator
- ALL software contained in this repository comes WITHOUT warranty of any kind. Use at your own risk

#### Current Microcontrollers
| Architecture         | Supported Platforms | Hardware Abstraction |
| -------------------- | ------------------- | -------------------- |
| Cortex-M4            | STM32F446           | STArduino, Custom    |
| Atmel Mega           | Arduino Uno WiFi V2 | Arduino              |
| Tensilica Xtensa LX6 | ESP32               | Arduino              |
| Cortex-M0            | STM32F103           | Arduino              | 

#### Projects List
- STM32 Blue Pill
	- Silly Frosh (7 Segment Rotating Sentences)
	- Temperature display
	- Serial Print Testing
- ESP32
	- 7 Segment Display Example
	- LED MOSFET driver
	- TCP Transmissions to server
- Arduino
	- Bluetooth Transmissions of Accelerometer Data
- STM32F446
	- FreeRTOS Bare Metal Template
	- Blank CMake Template
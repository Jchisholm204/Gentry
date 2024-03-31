# STM32 Basic Bootloader Test Project
*Proof of Concept*

This is a test project for writing an STM32 bootloader. There are two projects contained in this folder.

The bootloader itself, stored within the `bootloader/` directory is a 16KB project that works as a preloader. 16KB is the first sector of the flash on an STM32F446.

The main code includes the binary file generated from the bootloader project. The file is included in `loader.s` and is also placed in the beginning of flash in the linker script.

This project is not intended as a complete bootloader, only providing some of the framework required for making a custom bootloader. In the future, I hope to expand this project to work as a
complete bootloader with the ability to flash and monitor the main program.

### Current Program Behavior
The STM32 will first load the bootloader project. This project has some simple code to blink the LED's on the PM test board really fast. After about a second, the main program is loaded.
This program blinks the LEDs about every half second, according to the SysTick interrupt.
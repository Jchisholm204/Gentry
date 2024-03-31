# Bootloader Test Project

This is a test project for writing an STM32 bootloader. There are two projects contained in this folder.

The bootloader itself, stored within the `bootloader/` directory is a 16KB project that works as a preloader. 16KB is the first sector of the flash on an STM32F446.

The main code includes the binary file generated from the bootloader project. The file is included in `loader.s` and is also placed in the beginning of flash in the linker.

There are a number of issues with this project that will be addressed when I attempt to create a proper bootloader. This is for learning purposes only.
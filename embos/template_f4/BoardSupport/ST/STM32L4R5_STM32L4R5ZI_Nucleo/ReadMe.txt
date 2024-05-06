ReadMe.txt for the ST STM32L4R5ZI start project.

This project was built for emIDE V2.20.

Supported hardware:
===================
The sample project for the STM32L4R5ZI is prepared to run on
an ST STM32L4R5ZI Nucleo board. Using different target hardware
may require modifications.

Configurations:
===============
- Debug:
  This configuration is prepared for download into internal
  Flash using J-Link. An embOS debug and profiling library
  is used.
  To use SEGGER SystemView with this configuration, configure
  SystemViewer for STM32L4R5ZI as target device and SWD at
  2000 kHz as target interface.

- Release:
  This configuration is prepared for download into internal
  Flash using J-Link. An embOS release library is used.

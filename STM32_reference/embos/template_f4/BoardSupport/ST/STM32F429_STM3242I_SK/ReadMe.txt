ReadMe.txt for the ST STM32F429 start project.

This project was built for emIDE V2.20.

Supported hardware:
===================
The sample project for the ST STM32F429 is prepared to run on
a IAR STM32F42I SK eval board. Using different target
hardware may require modifications.

Configurations:
===============
- Debug:
  This configuration is prepared for download into internal
  Flash using J-Link. An embOS debug and profiling library is
  used.
  To use SEGGER SystemView with this configuration, configure
  SystemViewer for STM32F429II as target device and SWD at 2000 kHz
  as target interface.

- Release:
  This configuration is prepared for download into internal
  Flash using J-Link. An embOS release library is used.

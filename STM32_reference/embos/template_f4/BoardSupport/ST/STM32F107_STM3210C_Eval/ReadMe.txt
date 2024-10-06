ReadMe.txt for the ST STM32F107 start project.

This project was built for emIDE V2.20.

Supported hardware:
===================
The sample project for the ST STM32F107 is prepared to run on
a ST STM3210C eval board. Using different target
hardware may require modifications.

Configurations:
===============
- Debug:
  This configuration is prepared for download into internal
  Flash using J-Link. An embOS debug and profiling library is
  used.
  To use SEGGER SystemView with this configuration, configure
  SystemViewer for STM32F103RC as target device and SWD at
  2000 kHz as target interface.

- Release:
  This configuration is prepared for download into internal
  Flash using J-Link. An embOS release library is used.

Notes:
======
To use ethernet functionality please make sure that you set
jumper  JP4 (MCO) to 1-2 (left side / away from the ethernet jack).

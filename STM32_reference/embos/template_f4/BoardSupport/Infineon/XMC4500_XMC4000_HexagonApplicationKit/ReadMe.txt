ReadMe.txt for the Infineon XMC4500 start project.

This project was built for Atollic TrueSTUDIO for ARM V8.1.0.

Supported hardware:
===================
The sample project for the Infineon XMC4500 CPU is prepared
to run on an Infineon Hexagon Application Kit XMC4500 Series. 
Using different target hardware may require modifications.

Configurations:
===============
- Debug:
  This configuration is prepared for download into internal 
  Flash using J-Link. An embOS debug and profiling library 
  is used.
  To use SEGGER SystemView with this configuration while debugging,
  configure SystemView for Cortex-M4 as target device and SWD at
  2000 kHz as target interface.
  To use SEGGER SystemView with this configuration running stand-alone,
  configure SystemView for XMC4500-1024 as target device and SWD
  at 2000 kHz as target interface.

- Release:
  This configuration is prepared for download into internal
  Flash using J-Link. An embOS release library is used.

ReadMe.txt for the NXP LPC4357 start project.

The project Start_LPC4357.emP was built for emIDE V2.20.
The project .cproject was built for Atollic TrueSTUDIO for ARM V8.1.0.

Supported hardware:
===================
The sample projects for the NXP LPC4357 are prepared to run on a 
Keil MCB4357 eval board. Using different target hardware may 
require modifications.

Configurations:
===============
- Debug:
  This configuration is prepared for download into internal
  Flash using J-Link. An embOS debug and profiling library
  is used.
  To use SEGGER SystemView with this configuration, configure
  SystemView for LPC4357_M4 as target device and SWD at 2000 kHz
  as target interface.

- Release:
  This configuration is prepared for download into internal
  flash using J-Link. An embOS release library is used.

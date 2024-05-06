ReadMe.txt for the ST STM32F407 start project.

The Start_STM32F407.emP project was built for emIDE V2.20.
The .cproject project was built for Atollic TrueSTUDIO for ARM V8.1.0.

Supported hardware:
===================
The sample projects for the ST STM32F40G are prepared to 
run on an ST STM3240G-Eval eval board. Using different
target hardware may require modifications.

Configurations:
===============
- Debug:
  This configuration is prepared for download into internal
  Flash using J-Link. An embOS debug and profiling library
  is used.
  To use SEGGER SystemView with this configuration, configure
  SystemView for STM32F407IG as target device and SWD at
  2000 kHz as target interface.

- Release:
  This configuration is prepared for download into internal
  Flash using J-Link. An embOS release library is used.

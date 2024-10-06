ReadMe.txt for the Fujitsu MB9BF506 start project.

This project was built for Atollic TrueSTUDIO for ARM V8.1.0.

Supported hardware:
===================
The sample project for the Fujitsu MB9BF506N CPU is prepared
to run on a Fujitsu SK FM3 100PMC JLINK eval board. Using
different target hardware may require modifications.

Configurations:
===============
- Debug:
  This configuration is prepared for download into internal
  Flash using J-Link. An embOS debug and profiling library
  is used.
  To use SEGGER SystemView with this configuration, configure
  SystemView for MB9BF506N as target device and SWD at
  2000 kHz as target interface.

- Release:
  This configuration is prepared for download into internal
  Flash using J-Link. An embOS release library is used.

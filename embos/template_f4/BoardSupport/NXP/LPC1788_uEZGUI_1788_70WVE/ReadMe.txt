ReadMe.txt for the NXP LPC1788 start project.

This project was built for emIDE V2.20.

Supported hardware:
===================
The sample project for the NXP LPC1788 is prepared to run on
a FDI uEZGUI-1788-70WVE eval board. Using different target
hardware may require modifications.

Configurations:
===============
- Debug:
  This configuration is prepared for download into internal
  Flash using J-Link. An embOS debug and profiling library is
  used.
  To use SEGGER SystemView with this configuration, configure
  SystemViewer for LPC1788 as target device and SWD at 2000 kHz
  as target interface.

- Release:
  This configuration is prepared for download into internal
  Flash using J-Link. An embOS release library is used.

ReadMe.txt for the TI TM4C129 start project.

This project was built for emIDE V2.20.

Supported hardware:
===================
The sample project for the TI TM4C129 is prepared to run on
a TI Tiva TM4C129x Development Board. Using different target
hardware may require modifications.

Configurations:
===============
- Debug:
  This configuration is prepared for download into internal
  Flash using J-Link. An embOS debug and profiling library is
  used.
  To use SEGGER SystemView with this configuration, configure
  SystemViewer for TM4C1290NCZAD as target device and SWD at
  2000 kHz as target interface.

- Release:
  This configuration is prepared for download into internal
  Flash using J-Link. An embOS release library is used.

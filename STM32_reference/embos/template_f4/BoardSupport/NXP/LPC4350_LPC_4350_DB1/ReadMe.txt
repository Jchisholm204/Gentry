ReadMe.txt for the NXP LPC4350 start project.

This project was built for emIDE V2.20.

Supported hardware:
===================
The sample project for the NXP LPC4350 is prepared to run on
an Arrow LPC-4350-DB1 Development eval board. Using different target
hardware may require modifications.

Configurations:
===============
- Debug:
  This configuration is prepared for download into external NOR
  Flash using J-Link. An embOS debug and profiling library is
  used.
  To use SEGGER SystemView with this configuration, configure
  SystemViewer for LPC4350_M4 as target device and SWD at
  1000 kHz as target interface.

- Release:
  This configuration is prepared for download into external NOR
  flash using J-Link. An embOS release library is used.

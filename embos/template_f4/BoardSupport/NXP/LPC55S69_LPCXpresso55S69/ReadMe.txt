ReadMe.txt for the NXP LPC55S69 start project.

This project was built for emIDE V2.20.

Supported hardware:
===================
The sample project for the NXP LPC55S69 is prepared to run on
an NXP LPCXpresso55S69 Eval board. Using different target
hardware may require modifications.

Configurations:
===============
- Debug:
  This configuration is prepared for download into internal
  Flash using J-Link. An embOS debug and profiling library
  is used.
  To use SEGGER SystemView with this configuration, configure
  SystemViewer for LPC55S69 as target device and SWD at 4000 kHz
  as target interface. In addition, the RTT buffer address must
  be specified with this target (in case of doubt, search for
  _SEGGER_RTT in the respective mapfile).

- Release:
  This configuration is prepared for download into internal
  Flash using J-Link. An embOS release library is used.


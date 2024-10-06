ReadMe.txt for the Renesas R7FA6M4 start project.

This project was built for Renesas e²studio V2021-04 (21.4.0) and the
Flexible Software Package (FSP) for Renesas RA MCU Family.

Supported hardware:
===================
The sample project for the R7FA6M4 is prepared to run on an Renesas EK-RA6M4 eval board.
Using different target hardware may require modifications.

Configurations:
===============
- Debug:
  This configuration is prepared for download into internal Flash using the onboard J-Link.
  An embOS debug and profiling library is used.
  To use SEGGER SystemView with this configuration, configure
  SystemViewer for R7FA6M4 as target device and SWD at 2000 kHz as target interface.
  In addition, the RTT buffer address must be specified with this target (in case of doubt,
  search for _SEGGER_RTT in the respective mapfile).

- Release:
  This configuration is prepared for download into internal Flash using the onboard J-Link.
  An embOS release library is used.

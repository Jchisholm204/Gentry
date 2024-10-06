ReadMe.txt for the Renesas R7F0E01 start project.

This project was built for Renesas e2Studio V2020-10 and gcc-arm-none-eabi-9-2020-q2-update.

Supported hardware:
===================
The sample project for the R7F0E01 is prepared to run on an Renesas RE01 256KB eval board.
Using different target hardware may require modifications.

Configurations:
===============
- Debug:
  This configuration is prepared for download into internal Flash using J-Link.
  An embOS debug and profiling library is used.
  To use SEGGER SystemView with this configuration, configure
  SystemViewer for Cortex-M4 as target device and SWD at
  2000 kHz as target interface. In addition, the RTT buffer
  address must be specified with this target (in case of doubt,
  search for _SEGGER_RTT in the respective mapfile).

- Release:
  This configuration is prepared for download into internal Flash using J-Link.
  An embOS release library is used.

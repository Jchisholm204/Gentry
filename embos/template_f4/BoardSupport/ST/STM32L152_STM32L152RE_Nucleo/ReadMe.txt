ReadMe.txt for the ST STM32L152 start project

This project was built for emIDE V2.20.

Supported hardware:
===================
The sample project for the STM32L152 is prepared to run on
an ST STM32L152RE Nucleo board. Using different target
hardware may require modifications.

Configurations:
===============
- Debug:
  This configuration is prepared for download into internal
  Flash using J-Link. An embOS debug and profiling library
  is used.
  To use SEGGER SystemView with this configuration, configure
  SystemViewer for STM32L152RE as target device and SWD at
  2000 kHz as target interface. In addition, the RTT buffer
  address must be specified with this target (in case of doubt,
  search for _SEGGER_RTT in the respective mapfile).

- Release:
  This configuration is prepared for download into internal
  Flash using J-Link. An embOS release library is used.

Note:
=====
SEGGER offers a firmware upgrading the ST-LINK on-board on
the Nucleo and Discovery Boards. This firmware makes the
ST-LINK on-board compatible to J-Link OB. Please visit
https://www.segger.com/jlink-st-link.html for further
information.
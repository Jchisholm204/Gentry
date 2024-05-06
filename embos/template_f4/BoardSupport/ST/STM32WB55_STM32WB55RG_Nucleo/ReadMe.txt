ReadMe.txt for the ST STM32WB55 start project.

This project was built for STM32CubeIDE Version: 1.0.0 Build: 2872_20190423-2022.

Supported hardware:
===================
The sample project for the STM32WB55 is prepared to run on
an ST STM32WB55RG Nucleo board. Using different target
hardware may require modifications.

Configurations:
===============
- Debug:
  This configuration is prepared for download into internal
  Flash using J-Link. An embOS debug and profiling library
  is used.
  To use SEGGER SystemView with this configuration, configure
  SystemViewer for Cortex-M4 as target device and SWD at
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
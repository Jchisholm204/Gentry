ReadMe.txt for the ST STM32F429 start project.

The Start_STM32F429.emP project was built for emIDE V2.20.
The .cproject project was built for ST SystemWorkbench v2.3.

Supported hardware:
===================
The sample project for the STM32F429ZI is prepared to run on
a ST STM32F429ZI_Nucleo board. Using different target hardware
may require modifications.

Configurations:
===============
- Debug:
  This configuration is prepared for download into internal
  Flash using J-Link. An embOS debug and profiling library
  is used.
  To use SEGGER SystemView with this configuration, configure
  SystemViewer for STM32F429ZI as target device and SWD at
  4000 kHz as target interface.

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
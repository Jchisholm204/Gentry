ReadMe.txt for the ST STM32F429 start project.

This project was built for emIDE V2.20.

Supported hardware:
===================
The sample project for the ST STM32F429 is prepared to run on
a ST STM32F429I-Discovery eval board. Using different target
hardware may require modifications.

Configurations:
===============
- Debug:
  This configuration is prepared for download into internal
  Flash using J-Link. An embOS debug and profiling library is
  used.
  In this configuration a preprocessor define is set to use
  SEGGER SystemView in single-shot-mode (SEGGER_SYSVIEW_SINGLE_SHOT=1).

- Release:
  This configuration is prepared for download into internal
  Flash using J-Link. An embOS release library is used.

Note:
=====
SEGGER offers a firmware upgrading the ST-LINK on-board on the Nucleo and
Discovery Boards. This firmware makes the ST-LINK on-board compatible to
J-Link OB. See https://www.segger.com/jlink-st-link.html for further details
and the firmware download.

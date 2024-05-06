ReadMe.txt for the ST STM32L476 start project

The project Start_STM32L476.emP was built for emIDE V2.20.

The project .cproject was built for GNU MCU Eclipse 4.5.1 and GNU MCU Windows
Build Tools 2.12 using gcc-arm-none-eabi-7-2018-q2-update.

The Eclipse launch configurations require the J-Link debugging Eclipse
plug-in, which is available at: http://gnuarmeclipse.github.io/debug/jlink/

Supported hardware:
===================
The sample project for the STM32L476 is prepared to run on
a ST STM32L476RG Nucleo board. Using different target hardware
may require modifications.

Configurations:
===============
- Debug:
  This configuration is prepared for download into internal
  Flash using J-Link. An embOS debug and profiling library
  is used.
  To use SEGGER SystemView with this configuration, configure
  SystemViewer for STM32L476RG as target device and SWD at
  2000 kHz as target interface.

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


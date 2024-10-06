ReadMe.txt for the ST STM32H745 start project.

This project was built for ST STM32CubeIDE V1.11.2.

Supported hardware:
===================
The sample project for the STM32H745 is prepared to run on an ST STM32H745XI Discovery board.
Using different target hardware may require modifications.

Configurations:
===============
- Debug:
  This configuration is prepared for download into internal Flash using ST-Link.
  An embOS debug and profiling library is used.
  To use SEGGER SystemView with this configuration, configure SystemViewer for STM32H745XI_M7
  as target device and SWD at 4000 kHz as target interface.

- Release:
  This configuration is prepared for download into internal Flash using ST-Link.
  An embOS release library is used.

Note:
=====
This project uses an embOS library without the workaround for
the ARM erratum 837070. If you have a device with the ARM core
revision r0p0 or r0p1 please use the according embOS library
with the workaround.
Please have a look in the core/compiler specific embOS manual
for more details.
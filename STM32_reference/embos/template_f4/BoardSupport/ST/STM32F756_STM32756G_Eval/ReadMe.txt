ReadMe.txt for the ST STM32F756 start project.

The Start_STM32F756.emP project was built for emIDE V2.20.
The .cproject project was built for Atollic TrueSTUDIO for ARM V8.1.0.

Supported hardware:
===================
The sample projects for the ST STM32F756NG are prepared to
run on a ST STM32756G-Eval board. Using different target
hardware may require modifications.

Configurations:
===============
- Debug:
  This configuration is prepared for download into internal
  Flash using J-Link. An embOS debug and profiling library
  is used.
  To use SEGGER SystemView with this configuration, configure
  SystemView for STM32F756NG as target device and SWD at
  2000 kHz as target interface. In addition, the RTT buffer
  address must be specified with this target (in case of doubt,
  search for _SEGGER_RTT in the respective mapfile).

- Release:
  This configuration is prepared for download into internal
  Flash using J-Link. An embOS release library is used.

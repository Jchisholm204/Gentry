ReadMe.txt for the Microchip (Atmel) SAMV71Q21 start project.

This project was built for GNU MCU Eclipse 4.5.1 and GNU MCU Windows
Build Tools 2.12 using gcc-arm-none-eabi-7-2018-q2-update.

The Eclipse launch configurations require the J-Link debugging Eclipse
plug-in, which is available at: http://gnuarmeclipse.github.io/debug/jlink/

Supported hardware:
===================
The sample project for the Microchip (Atmel) SAMV71Q21 is prepared
to run on an Microchip (Atmel) SSAMV71 XPlainedUltra eval board,
but may be used on other target hardware as well.

Using different target hardware may require modifications.

Configurations
==============
- Debug:
  This configuration is prepared for download into internal
  Flash using J-Link. An embOS debug and profiling library is
  used.
  To use SEGGER SystemView with this configuration, configure
  SystemViewer for ATSAMV71Q21 as target device and SWD at
  4000 kHz as target interface.

- Release:
  This configuration is prepared for download into internal
  Flash using J-Link. An embOS release library is used.

ReadMe.txt for the Toshiba TMPM369 start project.

This project was built for Atollic TrueSTUDIO for ARM V8.1.0.

Supported hardware:
===================
The sample project for the Toshiba TMPM369FDFG is prepared
to run on a Toshiba BMSKTOPASM369 eval board. Using
different target hardware may require modifications.

Configurations:
===============
- Debug:
  This configuration is prepared for download into internal
  Flash using J-Link. An embOS debug and profiling library
  is used.
  To use SEGGER SystemView with this configuration, configure
  SystemView for TMPM369FDFG as target device and SWD at
  2000 kHz as target interface.

- Release:
  This configuration is prepared for download into internal
  Flash using J-Link. An embOS release library is used.

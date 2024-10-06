ReadMe.txt for the Hilscher netX90 start project.

This project was built for Hilscher netX Studio CDT 1.0700.1.3056.

Supported hardware:
===================
The sample project for the Hilscher netX90 CPU is prepared
to run on an Hilscher NXHX 90-JTAG board. Using different
target hardware may require modifications.

Configurations:
===============
- Debug:
  This configuration is prepared for download into internal Flash
  using the NXHX-90 On-Board Debugger. An embOS debug and profiling
  library is used.
  To use SEGGER SystemView with this configuration running stand-alone,
  connect a J-Link via the JTAG connector and configure SystemView for
  netX90_App as target device and SWD at 4000 kHz as target interface.
  In order to use the JTAG connector, switch the DIP switch S701-1 to ON.

- Release:
  This configuration is prepared for download into internal Flash
  using the NXHX-90 On-Board Debugger. An embOS release library is used.

Notes
=====
This project builds the elf and nai files for the app core and requires
the target device to already contain all needed com core firmwares and
configuration files for loading and starting the nai application. If the
device does not contain the needed files, please perform following steps:

 1.) Open the Flasher and select the interface
 2.) Select and download "InternalFlash01.bin" into the internal flash 01 (COM)
 3.) Select and download "InternalFlash2.bin" into the internal flash 2 (APP)



ReadMe.txt for the NXP (Freescale) Kinetis KE02Z 40MHz start project.

This project was built for MCUXpresso V10.2.0

Supported hardware:
===================
The sample project for the Kinetis KE02Z64 40MHz is prepared to run on
a NXP (Freescale) FRDM-KE02Z40M eval board. Using different target
hardware may require modifications.

Configurations:
===============
- Debug:
  This configuration is prepared for download into internal
  Flash using J-Link. An embOS debug and profiling library is
  used.
  To use SEGGER SystemView with this configuration, configure
  SystemViewer for MKE02Z64XXX4 as target device and SWD at
  2000 kHz as target interface.

- Release:
  This configuration is prepared for download into internal
  Flash using J-Link. An embOS release library is used.

Note:
=====
Due to 4KB RAM limitation, SystemView is disabled by default in the
RTOSInit*.c. You can enable it by defining OS_SYSVIEW_ENABLE to 1.
This will require you to resign RAM that is used by other resources
like for instance embOSView, RTT buffers, heap or stack.

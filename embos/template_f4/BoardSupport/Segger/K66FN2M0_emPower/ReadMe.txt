ReadMe.txt for the NXP (Freescale) K66 start project.

This project was built for emIDE V2.20.

Supported hardware:
===================
The sample project for the Kinetis K66 is prepared to run o
a Segger emPower eval board. Using different target
hardware may require modifications.

Note:
=====
Please make sure your device is unlocked before downloading your application.
To do so follow these steps:
- Connect your device with the J-Link
- Open the J-Link Commander
- Type:
  "unlock Kinetis"

Now the Kinetis device is unlocked and you are able to download your application

Configurations:
===============
- Debug:
  This configuration is prepared for download into internal
  Flash using J-Link. An embOS debug and profiling library is
  used.
  To use SEGGER SystemView with this configuration, configure
  SystemViewer for MK66FN2M0XXX18 as target device and SWD at
  2000 kHz as target interface.

- Release:
  This configuration is prepared for download into internal
  Flash using J-Link. An embOS release library is used.

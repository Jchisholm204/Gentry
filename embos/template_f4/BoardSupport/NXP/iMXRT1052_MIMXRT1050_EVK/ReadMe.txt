ReadMe.txt for the NXP (Freescale) iMXRT1052 start project.

This project was built for emIDE V2.20 and gcc version 8.2.1.

Supported hardware:
===================
The sample project for the iMXRT1052 is prepared to run on
an NXP MIMXRT1050-EVK(B) board. Using different target hardware
may require modifications.

Configurations:
===============
- Debug_RAM:
  This configuration is prepared for download into internal
  RAM using J-Link. An embOS debug and profiling library
  is used.
  To use SEGGER SystemView with this configuration,
  configure SystemView for MIMXRT1052XXXXB as target device and
  SWD at 2000 kHz as target interface.

- Release_RAM:
  This configuration is prepared for download into internal
  RAM using JLink. An embOS release library is used.

- Debug_Hyper_Flash:
  This configuration is prepared for download into external
  Hyper Flash using J-Link. An embOS debug and profiling library
  is used.
  To use SEGGER SystemView with this configuration,
  configure SystemView for MIMXRT1052XXXXB as target device and
  SWD at 2000 kHz as target interface.

- Release_Hyper_Flash:
  This configuration is prepared for download into external
  Hyper Flash using JLink. An embOS release library is used.

- Debug_QSPI_Flash:
  This configuration is prepared for download into external
  QSPI Flash using J-Link. An embOS debug and profiling library
  is used.
  To use SEGGER SystemView with this configuration,
  configure SystemView for MIMXRT1052XXXXB as target device and
  SWD at 2000 kHz as target interface.

- Release_QSPI_Flash:
  This configuration is prepared for download into external
  QSPI Flash using JLink. An embOS release library is used.

Notes:
======
This project uses an embOS library without the workaround for
the ARM erratum 837070. If you have a device with the ARM core
revision r0p0 or r0p1 please use the according embOS library
with the workaround.
Please have a look in the core/compiler specific embOS manual
for more details.

The LED implementation is disabled by default. In order to
enable the LED implementation, set BSP_ENABLE_LED to 1 in your
project's preprocessor settings or inside Setup/BSP.c.

Make sure that the newest version of the SEGGER J-Link software
is installed. Also, in order to boot from flash, some configurations
have to be done:

== Hyper Flash
Set the SW7 DIP switch to:

8       5
|-------|
| |x|x| |
|-------|
|x| | |x|
|-------|
1       4

If the changes for QSPI Flash were applied, then undo those changes.

== QSPI Flash
Set the SW7 DIP switch to:

8       5
|-------|
| | |x| |
|-------|
|x|x| |x|
|-------|
1       4

Open the JLinkDevices.xml in the installation directory of the SEGGER J-Link
software and search for the <Device> entry for the iMXRT105. Remove all
<Device> entries for the MXIMXRT105x devices by commenting them, so that it
can be added easily, if Hyper Flash shall be used again.

Note: Several hardware modifications need to be done on the evaluation board
to enable QSPI programming. For further information regarding this, please
refer to NXP's board user manual.

For further information, please take a look into the SEGGER wiki:
https://wiki.segger.com/i.MXRT1050

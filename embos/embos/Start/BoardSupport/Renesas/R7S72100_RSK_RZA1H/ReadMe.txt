ReadMe.txt for the Renesas R7S72100 start project.

This project was built for emIDE V2.20.

Supported hardware:
===================
The sample project for the R7S72100 is prepared to run on
a Renesas RSKRZA1 eval board. Using different target hardware
may require modifications.

Configurations:
===============
- Debug:
  This configuration is prepared for download into internal
  SRAM using J-Link. An embOS debug and profiling library is
  used.
  To use SEGGER SystemView with this configuration, configure
  SystemView for R7S721001 as target device and JTAG at
  4000 kHz as target interface. In addition, the RTT buffer
  address must be specified with this target (in case of doubt,
  search for _SEGGER_RTT in the respective mapfile).

- Debug_SPI_Flash:
  This configuration is prepared for download into external
  SPI Flash using J-Link. An embOS debug and profiling library
  is used.
  To use SEGGER SystemView with this configuration, configure
  SystemView for R7S721001 as target device and JTAG at
  4000 kHz as target interface. In addition, the RTT buffer
  address must be specified with this target (in case of doubt,
  search for _SEGGER_RTT in the respective mapfile).

- Release:
  This configuration is prepared for download into internal
  SRAM using J-Link. An embOS release library is used.

Notes:
======
Boot modes selectable via SW6:
NOR flash  ON-ON-ON-ON-ON-ON
QSPI flash OFF-ON-OFF-ON-ON-ON

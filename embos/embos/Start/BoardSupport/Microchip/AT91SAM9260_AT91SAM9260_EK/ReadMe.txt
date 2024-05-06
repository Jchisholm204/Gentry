Readme.txt for the Atmel AT91SAM9260 start project.

This project was built for emIDE V2.20.

Supported hardware:
===================
The sample project for the AT91SAM9260 is prepared to run on
an Atmel AT91SAM9260-EK eval board. Using different target
hardware may require modifications.

Configurations:
===============
- Debug:
  This configuration is prepared for download into SDRAM
  using J-Link. An embOS debug and profiling library is used.
  During startup, the MMU is configured to map the first
  megabyte of the SDRAM to 0x00.
  The GDB script file AT91SAM9260_SDRAM.gdb contains all
  steps required to initialize the CPU before downloading.

- Release:
  This configuration is prepared for download into SDRAM
  using J-Link. An embOS release library is used.
  During startup, the MMU is configured to map the first
  megabyte of the SDRAM to 0x00.
  The GDB script file AT91SAM9260_SDRAM.gdb contains all
  steps required to initialize the CPU before downloading.

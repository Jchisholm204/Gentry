Readme.txt for the Atmel AT91SAM9263 start project.

This project was built for emIDE V2.20.

Supported hardware:
===================
The sample project for AT91SAM9263 is prepared to run on
an Atmel AT91SAM9263-EK eval board. Using different target
hardware may require modifications.

Configurations:
===============
- Debug_iRAM:
  This configuration is prepared for download into internal
  RAM using J-Link. An embOS debug and profiling library is
  used.
  The GDB script file AT91SAM9263_iRAM.gdb contains all steps
  required to initialze the CPU before downloading

- Debug_SDRAM:
  This configuration is prepared for download into SDRAM
  using J-Link. An embOS debug and profiling library is used.
  During startup, the MMU is configured to map the first
  megabyte of the SDRAM to 0x00.
  The GDB script file AT91SAM9263_SDRAM.gdb contains all steps
  required to initialize the CPU before downloading.

- Release_iRAM:
  This configuration is prepared for download into internal
  RAM using J-Link. An embOS release library is used.
  The GDB script file AT91SAM9263_iRAM.gdb contains all steps
  required to initialze the CPU before downloading

- Release_SDRAM:
  This configuration is prepared for download into SDRAM
  using J-Link. An embOS release library is used.
  During startup, the MMU is configured to map the first
  megabyte of the SDRAM to 0x00.
  The GDB script file AT91SAM9263_SDRAM.gdb contains all steps
  required to initialize the CPU before downloading.

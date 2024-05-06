Readme.txt for the Altera Cyclone V SoC start project

This project was built for emIDE V2.20.

Supported hardware:
===================
The sample project for the Cyclone V is prepared to run on
a Altera SOCrates eval board. Using different target
hardware may require modifications.

Configurations:
===============
- Debug:
  This configuration is prepared for download into external
  SDRAM using J-Link. An embOS debug and profiling library is
  used.
  The second level bootloader u-boot-spl is required
  to initialize the PLL and Bus interface of the CPU.
  The GDB script file Start_AlteraSoC.gdb contains
  all steps to load and start the bootloader and loads
  the application.
  All commands of this files are also set in the project
  options for the bootloader.

- Release:
  This configuration is prepared for download into external
  SDRAM using J-Link. An embOS release library is used.
  The second level bootloader u-boot-spl is required
  to initialize the PLL and Bus interface of the CPU.
  The GDB script file Start_AlteraSoC.gdb contains
  all steps to load and start the bootloader and loads
  the application.
  All commands of this files are alos set in the project
  options for the bootloader.

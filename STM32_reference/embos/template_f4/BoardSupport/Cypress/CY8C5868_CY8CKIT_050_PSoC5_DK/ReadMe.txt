Readme.txt for the Cypress PSoC5 CY8C5868 (Cortex-M3) start project.

This project was built for PSoC Creator 4.2.

Supported hardware:
===================
The sample project for the CY8C5868 is prepared to
run on a CY8CKIT-050 PSoC5 DK development board but may
be used on other target hardware as well.

Configurations
==============
- Debug:
  This configuration is prepared for download into internal
  Flash using CYPRESS DVKProg5. An embOS debug and profiling
  library is used.
  To use SEGGER SystemView with this configuration, connect a
  J-Link to the board and configure SystemView for CY8C5868XXXLP
  as target device and SWD at 2000 kHz as target interface.
  In addition, the RTT buffer address must be specified with this
  target (in case of doubt, search for _SEGGER_RTT in the respective
  mapfile). Also, the board might need to be power-cycled before
  using an external J-Link after the onboard CYPRESS DVKProg5
  has been used.

- Release:
  This configuration is prepared for download into internal
  Flash using CYPRESS DVKProg5. An embOS release library is used.

Using different target hardware may require modifications.
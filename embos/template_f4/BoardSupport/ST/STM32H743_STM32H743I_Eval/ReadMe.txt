ReadMe.txt for the ST STM32H743 start project.

The Start_STM32H743.emP project was built for emIDE V2.20.
The SystemWorkbench/.cproject project was built for ST SystemWorkbench v2.3.
The Atollic_TrueSTUDIO/.cproject project was built for Atollic TrueSTUDIO for ARM V8.1.0.

Supported hardware:
===================
The sample projects for the ST STM32H743XI are prepared to
run on an ST STM32H743I-EVAL board. Using different target
hardware may require modifications.

Configurations:
===============
- Debug:
  This configuration is prepared for download into internal
  Flash using J-Link. An embOS debug and profiling library is
  used.
  To use SEGGER SystemView with this configuration, configure
  SystemView for stm32h743XI as target device and SWD at
  2000 kHz as target interface.

- Release:
  This configuration is prepared for download into internal
  Flash using J-Link. An embOS release library is used.

Note:
=====
This project uses an embOS library without the workaround for
the ARM erratum 837070. If you have a device with the ARM core
revision r0p0 or r0p1 please use the according embOS library
with the workaround.
Please have a look in the core/compiler specific embOS manual
for more details.
ReadMe.txt for the Renesas R7FA6M4 TrustZone start project.

This project was built for Renesas e²studio V2021-04 (21.4.0) and the
Flexible Software Package (FSP) for Renesas RA MCU Family.

Supported hardware:
===================
The sample project for the R7FA6M4 is prepared to run on an Renesas EK-RA6M4 eval board.
Using different target hardware may require modifications.

Description:
============
The folder secure and non_secure contain the according projects for the secure and non secure state.
Please import both projects into e2Studio and build the secure project first.
Thereafter please build the non secure project and start the according debug session.

The secure project code runs after reset. It initializes the SAU and starts the non secure project application.
The secure project also includes some functions in Arm_TZ_SecureFunctions.c to read and write secure register from the non secure state.
embOS runs completely in the non secure state but tasks can call functions from the secure state like IncrementCounter_s() in App_secure.c.
This sample application creates two tasks, each one calling OS_TASK_Delay().
The HPTask() calls the secure function IncrementCounter_s() which simply increments the secure counter Counter_s.
HPTask() also increments the non secure counter Counter_ns.

This demo shows that embOS is running in non secure state while embOS is able to call secure functions from tasks.

Configurations:
===============
- Debug:
  This configuration is prepared for download into internal Flash using the onboard J-Link.
  An embOS debug and profiling library is used.
  To use SEGGER SystemView with this configuration, configure
  SystemViewer for R7FA6M4AF as target device and SWD at 2000 kHz as target interface.
  In addition, the RTT buffer address must be specified with this target (in case of doubt,
  search for _SEGGER_RTT in the respective mapfile).

#**********************************************************************
#*               SEGGER MICROCONTROLLER GmbH GmbH & Co KG             *
#*       Solutions for real time microcontroller applications         *
#**********************************************************************
#*                                                                    *
#*       (c) 1995 - 2007  SEGGER Microcontroller GmbH & Co KG         *
#*                                                                    *
#*       www.segger.com     Support: support@segger.com               *
#*                                                                    *
#**********************************************************************
#*                                                                    *
#*       embOS * Real time operating system for microcontrollers      *
#*                                                                    *
#*                                                                    *
#*       Please note:                                                 *
#*                                                                    *
#*       Knowledge of this file may under no circumstances            *
#*       be used to write a similar product or a real-time            *
#*       operating system for in-house use.                           *
#*                                                                    *
#*       Thank you for your fairness !                                *
#*                                                                    *
#**********************************************************************
#*                                                                    *
#*       OS version: 3.88                                             *
#*                                                                    *
#**********************************************************************
#----------------------------------------------------------------------
#File   : AT91SAM7_FLASH_JLink.gdb
#Purpose: GDB command file for Atmel SAM7 series +
#                              J-Link (J-Link GDB Server)
#                              FLASH is mapped at 0x00000000
#-------- END-OF-HEADER -----------------------------------------------
#
# Connect to the J-Link gdb server
target remote localhost:2331
# Setup flash device
monitor flash device = AT91SAM7S256
# Set gdb server to little endian
monitor endian little
# Set speed to JTAG speed 12000
monitor speed 12000
# Reset target
monitor reset 0
# Set a break at main()
break main
# Load the ELF-file
load
# Let the application run to main()
continue

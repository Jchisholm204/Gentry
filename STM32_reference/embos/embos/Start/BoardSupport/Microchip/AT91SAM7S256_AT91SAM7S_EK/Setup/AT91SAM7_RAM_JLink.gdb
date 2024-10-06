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
#
#----------------------------------------------------------------------
#File   : AT91SAM7_RAM_JLink.gdb
#Purpose: GDB command file for Atmel SAM7 series +
#                              J-Link (J-Link GDB Server)
#                              RAM is mapped at 0x00000000
#-------- END-OF-HEADER -----------------------------------------------
#
# Connect to the J-Link gdb server
target remote localhost:2331
# Set gdb server to little endian
monitor endian little
# Do a software reset for ATMEL AT91SAM7 MCUs.
monitor reset 8
# Wait and set JTAG speed to 30kHz.
monitor sleep 10
monitor speed 30
# Perform peripheral reset
monitor long 0xFFFFFD00 = 0xA5000004
monitor sleep 10
# Disable watchdog
monitor long 0xFFFFFD44 = 0x00008000
monitor sleep 10
# Initialize PLL
monitor long 0xFFFFFC20 = 0x00000601
monitor sleep 10
monitor long 0xFFFFFC2C = 0x00480a0e
monitor sleep 10
monitor long 0xFFFFFC30 = 0x00000007
monitor sleep 10
monitor long 0xFFFFFF60 = 0x00480100
monitor sleep 100
# Change mapping, putting RAM at addr. 0
monitor long 0xFFFFFF00 = 0x00000001
# Setup GDB for faster downloads
set remote memory-write-packet-size 1024
set remote memory-write-packet-size fixed
# Set to 12 MHz JTAG speed.
monitor speed 12000
# Set a break at main() 
break main
# Load the ELF-file
load
# Let the application run to main()
continue
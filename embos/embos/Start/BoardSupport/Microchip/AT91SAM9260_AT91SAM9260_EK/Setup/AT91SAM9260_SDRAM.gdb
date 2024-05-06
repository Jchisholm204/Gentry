######################################################################
#
#       GDB init file for AT91SAM9260-EK board 
#       with AT91SAM9260 CPU and external SDRAM
#       used with J-Link
#
monitor reset
# Give debugger some information about memory
monitor exec map region 0x200000-0x200FFF C
monitor exec map region 0x300000-0x300FFF C
monitor exec map region 0x20000000-0x23FFFFFF C
#
# Ensure JTAG speed is not too high, use adaptive clocking
#
monitor speed adaptive
#
# Perform a peripheral reset (NOT CPU reset)
#
monitor memU32 0xFFFFFD00 = 0xA5000004
#
# Disable watchdog
#
monitor memU32 0xFFFFFD44 = 0x00008000
#
# Enable main oscillator to speed up CPU
#
monitor memU32 0xFFFFFC20 = 0x0000601
monitor sleep 10
#
# Switch CPU to master clock
#
monitor memU32 0xFFFFFC30 = 0x00000001
monitor sleep 1
#
# Allow user reset
#
monitor memU32 0xFFFFFD08 = 0xA5000001
#
# Initialize AIC, disable all interrupts
#
monitor memU32 0xFFFFF124 = 0xFFFFFFFF
#
# Initialize BUS interface and SDRAM
# Configure PIOD as peripheral (D16/D31)
#
monitor memU32 0xFFFFEF1C = 0x00010002
monitor memU32 0xFFFFF870 = 0xFFFF0000
monitor memU32 0xFFFFF804 = 0xFFFF0000
#
# Now initialize SDRAM
#
monitor memU32 0xFFFFEA08 = 0x85227259
monitor sleep 2
monitor memU32 0xFFFFEA00 = 0x00000002
monitor memU32 0x20000000
monitor sleep 2
monitor memU32 0xFFFFEA00 = 0x00000004
monitor memU32 0x20000000
monitor memU32 0xFFFFEA00 = 0x00000004
monitor memU32 0x20000000
monitor memU32 0xFFFFEA00 = 0x00000004
monitor memU32 0x20000000
monitor memU32 0xFFFFEA00 = 0x00000004
monitor memU32 0x20000000
monitor memU32 0xFFFFEA00 = 0x00000004
monitor memU32 0x20000000
monitor memU32 0xFFFFEA00 = 0x00000004
monitor memU32 0x20000000
monitor memU32 0xFFFFEA00 = 0x00000004    
monitor memU32 0x20000000
monitor memU32 0xFFFFEA00 = 0x00000004
monitor memU32 0x20000000

monitor memU32 0xFFFFEA00 = 0x00000003
monitor memU32 0x20000000 = 0xcafedede
monitor memU32 0xFFFFEA04 = 0x0000030C
monitor memU32 0xFFFFEA00 = 0x00000000
monitor memU32 0x20000000
#
# Finally load the executable
#
load

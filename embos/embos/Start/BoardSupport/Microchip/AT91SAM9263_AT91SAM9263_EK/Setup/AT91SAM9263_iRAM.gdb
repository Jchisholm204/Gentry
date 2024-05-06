######################################################################
#
#       GDB init file for AT91SAM9263-EK board 
#       with AT91SAM9263 CPU and internal RAM
#       used with J-Link
#
monitor reset
# Give debugger some information about memory
monitor exec map region 0x300000-0x31FFFF C
monitor exec map region 0x000000-0x01FFFF C
#
# Ensure JTAG speed is not too high, use adaptive clocking
#
monitor speed adaptive
#
# Perform a peripheral reset (NOT CPU reset)
#
monitor memU32 0xFFFFFD00 = 0xA5000004
#
# Enable main oscillator to speed up CPU
#
monitor memU32 0xFFFFFC20 = 0x00004001
monitor sleep 10
#
# Switch CPU to master clock
#
monitor memU32 0xFFFFFC30 = 0x00000001
monitor sleep 1
#
# Disable watchdog
#
monitor memU32 0xFFFFFD44 = 0x00008000
#
# Allow user reset
#
monitor memU32 0xFFFFFD08 = 0xA5000001
#
# Finally load the executable
#
load

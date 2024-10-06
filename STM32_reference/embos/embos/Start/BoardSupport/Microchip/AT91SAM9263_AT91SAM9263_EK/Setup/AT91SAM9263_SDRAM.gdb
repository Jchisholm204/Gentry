######################################################################
#
#       GDB init file for AT91SAM9263-EK board 
#       with AT91SAM9263 CPU and external SDRAM
#       used with J-Link
#
monitor reset
# Give debugger some information about memory
monitor exec map region 0x300000-0x31FFFF C
monitor exec map region 0x000000-0x01FFFF C
monitor exec map region 0x20000000-0x21FFFFFF C
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
# Initialize BUS interface and SDRAM
# Configure PIOD as peripheral (D16/D31)
#
monitor memU32 0xFFFFF870 = 0xFFFF0000
monitor memU32 0xFFFFF874 = 0x00000000
monitor memU32 0xFFFFF804 = 0xFFFF0000
# Configure EBI
monitor memU32 0xFFFFED20 = 0x00010002
# Now initialize SDRAM
monitor memU32 0xFFFFE208 = 0x85227259
monitor sleep 1
monitor memU32 0xFFFFE200 = 0x00000002
monitor memU32 0x20000000 = 0x00000000
monitor sleep 1
monitor memU32 0xFFFFE200 = 0x00000004
monitor memU32 0x20000000 = 0x00000001
monitor memU32 0xFFFFE200 = 0x00000004
monitor memU32 0x20000000 = 0x00000000
monitor memU32 0xFFFFE200 = 0x00000004
monitor memU32 0x20000000 = 0x00000000
monitor memU32 0xFFFFE200 = 0x00000004
monitor memU32 0x20000000 = 0x00000000
monitor memU32 0xFFFFE200 = 0x00000004
monitor memU32 0x20000000 = 0x00000000
monitor memU32 0xFFFFE200 = 0x00000004
monitor memU32 0x20000000 = 0x00000000
monitor memU32 0xFFFFE200 = 0x00000004    
monitor memU32 0x20000000 = 0x00000000
monitor memU32 0xFFFFE200 = 0x00000004
monitor memU32 0x20000000 = 0x00000000
monitor memU32 0xFFFFE200 = 0x00000003
monitor memU32 0x20000020 = 0xcafedede
monitor memU32 0xFFFFE204 = 0x000002BC
monitor memU32 0xFFFFE200 = 0x00000000
monitor memU32 0x20000000 = 0x00000000
#
# Finally load the executable
#
load

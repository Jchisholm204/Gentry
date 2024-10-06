#
# Reset and stop the system.
#
monitor reset

#
# Load the SPL preloader into memory.
#
file "Setup/u-boot-spl"
load

#
# Set a breakpoint in the SPL function spl_boot_device(). This function is
# called right before the SPL tries to load the next stage in the preloader.
#
tbreak spl_boot_device

#
# Set the PC to the entry point and go.
#
continue

#
# Wait for the breakpoint.
#

#
# Load the demo program.
#
file "Output/Debug/exe/Start_AlteraSoC.elf"
load

#
# Set processor mode
#
set $cpsr=0xC0000013

#
# Run the target and break at main().
#
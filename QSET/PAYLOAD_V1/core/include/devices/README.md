# Devices
This folder contains drivers for various devices the board may be required to interface with.

All abstraction layers within this folder must rely on ONLY the STM32 drivers.
Calls should not be made directly to the HAL.
The exception to this rule is the GPIO (General Purpose Input Output) HAL.

If you do end up requiring access to the HAL (Not the GPIO HAL),
good luck debugging your race conditions.

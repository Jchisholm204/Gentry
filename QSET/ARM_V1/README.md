# QSET Arm Control Board

USB Operated control board for the QSET 2025 Arm.

- Designed to interface with AK Series motors, servos and limit switches.
- Control interfaces are exposed over a USB CDC interface build with the IAD mechanism.
- Virtual COM port implemented for error feedback through USB CDC (IAD)

### USB Device Specifications
(exposed from the viewpoint of the host as per USB IF specifications)
- Vendor ID: 0xFFFE
- Product ID: 0x0A4D
- Virtual COM port (CDC)
    - NTF Interface Number: 0x00
    - Data Interface Number: 0x01
    - Data RX EP: 0x01
    - Data TX EP: 0x81
    - Maximum Data Packet Size: 0x40
    - NTF EP: 0x82
    - Maximum NTF Packet Size: 0x08
- Control Endpoint (CDC)
    - NTF Interface Number: 0x02
    - Data Interface Number: 0x03
    - Data RX EP: 0x03
    - Data TX EP: 0x83
    - Maximum Data Packet Size: 0x40
    - NTF EP: 0x84
    - Maximum NTF Packet Size: 0x08

### CAN Bus Specifications
- STM32 bxCAN (Interface Number 1)
- Designed for operation at 1000 KBPS
- Automatic Retransmission Disabled
- BTR configuration for 42Mhz (USB) or 45Mhz AHB1 clock skews
- Bus Filtering Disabled
- Hardware controlled mailboxes with dynamic interfaces

## Firmware
The Arm Firmware is split into three categories: OS, Drivers, and User Code.

### OS
Files part of the OS include the FreeRTOS Scheduler, `C` files under the OS folder in `src`, and the HAL.
Additionally, the `FreeRTOSConfig.h` file under `include/config`.

Any file falling in the OS category should not be modified.
These files allow the base system to function correctly.
Modifying them could introduce unintended errors or faults that are difficult to diagnose.

### Drivers
Drivers are the middleware.
They exist to provide access control between FreeRTOS tasks and the STM32 hardware.
The QSET Arm Control Board utilizes UART, CAN, and USB Drivers.

#### UART
The UART driver, titled `Serial`, acts as a middleware allowing UART ports to be used as files.
This enables `C` functions such as `printf` or `fprintf`.
Reading from a UART port using a file wrapper is currently not possible.
To read from a UART port, a task must attach a stream buffer to the port.
That task can then read characters from the FreeRTOS stream buffer.

#### CAN
The CAN Bus driver allows tasks to register `CanMailbox_t` structures with it.
A mailbox is tailored to a specific CAN ID.
When a CAN Bus interrupt occurs, the message is forwarded to the CAN Task, which then deposits messages into mailboxes.
Tasks can read from the mailbox at any time, or block waiting for an RX event with `can_mailbox_wait`.
Tasks may choose to block indefinitely waiting for an RX event.
If blocking waiting for an RX event, the task will be woken up once the CAN task has finished.
The CAN task can register a maximum of `MAX_MAILBOXES`.
Adjust this define statement to enable more mailboxes.
This will use additional memory.

#### USB
The USB Drivers used in this project are from the libusb_stm32 library.
Most of the USB Driver and HAL are directly taken from the USB CDC Demo project.
Only minor modifications have been made to the library in this project.
See [this GitHub repository for more information.](https://github.com/dmitrystu/libusb_stm32)

### User Code
As a USB interface, the bulk of the code is present within`main.c`.
The USB Task handles limit switches and driving servos.

Motor Control is handled by the `mtr_ctrl` task.
Multiple instances of this task can be spawned through the `mtrCtrl_init` function.
Motor types and CAN ID's must be set through this function.

If a motor type is not present, it must be added to `include/AKMotor/AkMotor_constants.h`.
First, the type of motor must be added to the `AKMotorType` enum.
Secondly, the constants for the new motor type can be specified in the `AKConfigs` array.


## Driver
A libUSB driver has been created for interfacing with the Arm Board.
See `./driver` for more details.
See `driver/test` for a sample program on how to use the Arm Board Driver.


The driver is split into three folders:
- `include`:
    - contains files common to the firmware and the driver.
    - only `libusb_arm.h` must be included to use the driver.
- `lib`: Contains C files for the driver code
- `test`: Contains a test program for using the driver

### Using the Driver
Using the driver requires `libusb1.0`. Install on Ubuntu with:

```shell
sudo apt-get install libusb-1.0-0-dev
```

Clone/Copy the files from `include` and `lib`.
Symlinking to this repository is best as it ensures that changes to the firmware files will be present in the application code.


After Symlinking, follow the example code in `test` to interface with the board.
Most of the functions in the library are simple wrappers around `libusb1.0`.


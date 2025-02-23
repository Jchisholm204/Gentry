# QSET USB Evaluation Kit

- Mimics the Drive Board functionality over the USB interface.
- USB control inputs printed out over the ST-LINK virtual com port
- Designed for use with the Nucleo 446 Evaluation board

### USB Device Specifications
(exposed from the viewpoint of the host as per USB IF specifications)
- (Same as the drive board)
- Vendor ID: 0xFFFE
- Product ID: 0xD415
- Drive Motor + Auto Lights Control Interface (CDC)
    - NTF Interface Number: 0x00
    - Data Interface Number: 0x01
    - Data RX EP: 0x01
    - Data TX EP: 0x81
    - Maximum Data Packet Size: 0x40
    - NTF EP: 0x82
    - Maximum NTF Packet Size: 0x08
- Camera Boom Control Interface (CDC)
    - NTF Interface Number: 0x02
    - Data Interface Number: 0x03
    - Data RX EP: 0x03
    - Data TX EP: 0x83
    - Maximum Data Packet Size: 0x40
    - NTF EP: 0x84
    - Maximum NTF Packet Size: 0x08
- ADC + Sensor Data Interface (CDC)
    - NTF Interface Number: 0x04
    - Data Interface Number: 0x05
    - Data RX EP: 0x05
    - Data TX EP: 0x85
    - Maximum Data Packet Size: 0x40
    - NTF EP: 0x86
    - Maximum NTF Packet Size: 0x08


## Firmware
The Firmware is split into three categories: OS, Drivers, and User Code.

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

#### USB
The USB Drivers used in this project are from the libusb_stm32 library.
Most of the USB Driver and HAL are directly taken from the USB CDC Demo project.
Only minor modifications have been made to the library in this project.
See [this GitHub repository for more information.](https://github.com/dmitrystu/libusb_stm32)
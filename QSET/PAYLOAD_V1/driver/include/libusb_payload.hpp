/**
 * @file libusb_payload.hpp
 * @author Jacob Chisholm (Jchisholm204.github.io)
 * @brief USB Interface Library for QSET Chassis
 * @version 0.1
 * @date 2025-02-24
 * 
 * @copyright Copyright (c) 2023
 *
 *
 */
#ifndef _LIBUSB_PAYLOAD_H_
#define _LIBUSB_PAYLOAD_H_

extern "C"{
#include "usb_payload_defs.h"
#include "usb_dev.h"
#include "usb_packet.h"
#include <libusb-1.0/libusb.h>
}


#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <unordered_map>

class PayloadControlBoard {
    public:
        PayloadControlBoard(void);
        ~PayloadControlBoard(void);
        /**
         * @brief Establish a connection with the Payload Control Board
         *
         * @param blocking Set to true to block waiting for the device to be connected
         * @return libusb error or 0
         */
        int connect(bool blocking = false);
        /**
         * @brief Check if the connection to the board is active
         *
         * This function polls the board for a status packet. If reconnect is set to true,
         *  the class will attempt to reconnect to the board.
         *  
         * The default behavior of this function is blocking. This function should
         *  be called before attempting to communicate with the board.
         *  If non-blocking behavior is desired, it can be turned off. In this
         *  case, the function simply returns the internal connection state:
         *      connection_active
         *
         * @param reconnect Attempt to reconnect to the board
         * @param blocking Reconnection attempts will be blocking
         * @returns True if the connection is active and ready to transmit
         */
        bool is_active(bool reconnect = true, bool blocking = true);
        /**
         * @brief Get the Status from the payload board.
         *  
         *  This function will return ePayloadNoConnection if the connection to
         *   the board was lost.
         *
         * @return internal status value
         */
        ePayloadStatus get_status(void);
        /**
         * @brief Get the Status from the payload board.
         *  
         *  This function will return ePayloadNoConnection if the connection to
         *   the board was lost.
         *
         * @param msg The status message last sent by the board
         * @return internal status value
         */
        ePayloadStatus get_status(std::string &msg);
        /**
         * @brief Get the Stepper Driver Status Packet
         *  
         * @param n Stepper number of data to return
         *  Will return the first stepper if range is invalid
         */
        struct udev_stepper_info get_stepperInfo(enum ePayloadStepper n = eStepper1);
        /**
         * @brief Get the ADC voltage reading
         */
        float get_ADC(void);
        /**
         * @brief Get the internal temperature of the STM32 Chip (deg C)
         */
        float get_intTemp(void);
        /**
         * @brief Get the temperature from the BME680 (deg C)
         */
        float get_bmeTemp(void);
        /**
         * @brief Get the pressure from the BME680 (hPa)
         */
        float get_bmePressure(void);
        /**
         * @brief Get the humidity from the BME680 (%RH)
         */
        float get_bmeHumidity(void);
        /**
         * @brief Get the reported gas resistance from the BME680 (Ohms)
         */
        float get_bmeGasRest(void);
        /**
         * @brief Set teh stepper Configuration
         *
         * @param holdCurrent Hold Current (0-31)
         * @param runCurrent Run Current (0-31)
         * @param holdDelay Hold Delay (0-31)
         */
        void set_steperConfig(uint8_t holdCurrent, uint8_t runCurrent, uint8_t holdDelay);
        /**
         * @brief Set the stepper movement
         *
         * @param speed Speed to move at
         * @param position Number of steps to move
         */
        void set_stepper(int32_t speed, uint32_t position);
        /**
         * @brief Set the servo
         *  Default behavior is to set servo to neutral position
         *
         * @param servo Servo ID to set
         * @param value PWM value to set the servo to (us)
         */
        void set_servo(enum ePayloadServo servo, uint32_t value = 1500);
        /**
         * @brief Set the motor
         *  Default behavior is to turn the motor off
         *
         * @param motor Motor to set
         * @param power Power of the motor
         */
        void set_motor(enum ePayloadMotor motor, int8_t power = 0);
        /**
         * @brief Set the light control
         *  Default behavior is to turn all lights off
         *
         * @param light to set to on
         * @param percent_power The Percent power applied to the light (10-100%)
         */
        void set_light(enum ePayloadLight light = eLightAllOFF, float percent_power = 100.0);
        // Latest LibUSB Error number
        int lusb_err;
        // Map enum to string
        std::unordered_map<enum ePayloadStatus, std::string> statusString = {
            {ePayloadOK, "OK"},
            {ePayloadUnInit, "Uninitialized"},
            {ePayloadIniFail, "Init Failed"},
            {ePayloadI2CFail, "I2C Failed"},
            {ePayloadFail, "General Failure"},
            {ePayloadNoConnection, "No Connection"}
        };
    private:
        int transmit(struct udev_pkt_ctrl *pkt);
        int pkt_poll(void);
        bool is_dev_connected(void);
        int reconnect(void);
        bool connection_active;
        int dev_init(int inum);
        struct udev_pkt_info pkt_info;
        libusb_context *lusb_ctx;
        libusb_device_handle *lusb_devHndl; 
        enum ePayloadStatus status;
};


#endif

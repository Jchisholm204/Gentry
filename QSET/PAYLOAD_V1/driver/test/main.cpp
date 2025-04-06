/**
 * @file main.cpp
 * @author Jacob Chisholm (Jchisholm204.github.io)
 * @brief Payload Test Code
 * @version 0.1
 * @date 2025-04-05
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include "libusb_payload.hpp"
#include <unistd.h>
#include <stdbool.h>
#include <memory.h>
#include <libusb-1.0/libusb.h>
#include <iostream>

int main(void){
    std::cout << "Running...\n";
    PayloadControlBoard pcb;
    std::cout << "Connecting to Board\n";
    pcb.connect(true);
    if(!pcb.is_active(true, false)){
        std::cout << "Connecting to Board\n";
        sleep(1);
    }
    std::cout << "Connected to Board: Success!!\n";
    int dir = 1;
    for(;;){
        if(!pcb.is_active(true, false)){
            std::cout << "Connection Lost: Reconnecting to Board\n";
            sleep(1);
            continue;
        }
        pcb.set_motor(eMotor1, 127); 
        switch(dir){
            case -1:
                dir = 0;
                break;
            case 0:
                dir = 1;
                break;
            case 1:
                dir = -1;
                break;
        }
        // Get the board status
        std::string msg;
        enum ePayloadStatus s = pcb.get_status(msg);
        // Optional, erase the newline character in the msg
        if(msg.find("\n") != std::string::npos)
            msg.erase(msg.find("\n"), 1);
        std::cout << msg << ": Status = " << pcb.statusString[s].c_str() << dir << "\n";
        sleep(1);
    }

    return 0;
}

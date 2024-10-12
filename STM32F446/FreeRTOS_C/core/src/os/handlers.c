/**
 * @file handlers.c
 * @author Jacob Chisholm (Jchisholm204.github.io)
 * @brief Hard Fault Handlers
 * @version 0.1
 * @date 2023-11-06
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "os/handlers.h"

// void HardFault_Handler(void){
//     // for(;;) __ASM("nop");
// }

void MemManage_Handler(void){
    for(;;) __ASM("nop");
}

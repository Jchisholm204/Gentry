/**
 * @file pin_cfg.h
 * @author Jacob Chisholm (https://Jchisholm204.github.io)
 * @brief Pin Configuration file
 * @version 0.1
 * @date 2024-10-09
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef _PIN_CFG_H_
#define _PIN_CFG_H_

#include "os/hal/hal_gpio.h"

// Define the board to use
#define BOARD_MOCKECU

#if defined(BOARD_MOCKECU)
    #include "os/config/boards/mockecu.h"
#else
    #include "os/config/boards/mockecu.h"
#endif

#endif

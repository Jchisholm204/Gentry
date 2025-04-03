/**
 * @file pin_cfg.h
 * @author Jacob Chisholm (https://Jchisholm204.github.io)
 * @brief Pin Configuration file
 * @version 0.1
 * @date 2024-10-09
 * 
 *
 * Use this file to define which board to use
 * Allows code to be easily moved between different boards
 *
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef _PIN_CFG_H_
#define _PIN_CFG_H_

// Define the board to use
// #define BOARD_MOCKECU
// #define BOARD_ARMV1
#define BOARD_CHASSISV1

#if defined(BOARD_MOCKECU)
    #include "config/boards/mockecu.h"
#elif defined(BOARD_NUCLEOZE)
    #include "config/boards/nucleoze.h"
#elif defined(BOARD_ARMV1)
    #include "config/boards/armv1.h"
#elif defined(BOARD_CHASSISV1)
    #include "config/boards/chassisv1.h"
#endif

#ifndef _BOARD_CONFIG_
    #error "No board has been defined.. Please define pin config"
#endif

#endif

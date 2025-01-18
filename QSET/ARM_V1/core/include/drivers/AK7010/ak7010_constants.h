/**
 * @file ak7010_constants.h
 * @author Jacob Chisholm (https://Jchisholm204.github.io)
 * @brief AK70-10 Constants
 * @version 0.1
 * @date 2025-01-18
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#ifndef _AK7010_CONSTANTS_H_
#define _AK7010_CONSTANTS_H_

/// @brief Minimum position value returned by the ak70-10 motors (rad)
#define AK_P_MIN -12.5

/// @brief Maximum position value returned by the ak70-10 motors (rad)
#define AK_P_MAX 12.5

/// @brief Minimum Velocity value returned by the ak70-10 motors (rad/s)
#define AK_V_MIN -50

/// @brief Maximum Velocity value returned by the ak70-10 motors (rad/s)
#define AK_V_MAX 50

/// @brief Minimum torque value accepted by the ak70-10 motors (Nm)
#define AK_T_MIN -25

/// @brief Maximum torque value accepted by the ak70-10 motors (Nm)
#define AK_T_MAX 25

/// @brief Minimum kp value accepted by ak70-10 motors
#define AK_KP_MIN 0

/// @brief Maximum kp value accepted by ak70-10 motors
#define AK_KP_MAX 500

/// @brief Minimum kd value accepted by ak70-10 motors
#define AK_KD_MIN 0

/// @brief Maximum kd value accepted by ak70-10 motors
#define AK_KD_MAX 5

/// @brief Max current draw of ak70-10 motors (Amps) (we have limited this to 20
/// Amps in on the motor firmware)
#define AK_I_MAX 60

/// @brief Minimum tempature value returned by ak70-10 motors (°C)
#define AK_Temp_MIN  -20

/// @brief Maximum tempature value returned by ak70-10 motors (°C)
#define AK_Temp_MAX  127

#endif

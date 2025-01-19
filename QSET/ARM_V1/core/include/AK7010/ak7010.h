/**
 * @file ak7010.h
 * @author Jacob Chisholm (https://Jchisholm204.github.io)
 * @brief AK70-10 Motor Driver
 * @version 0.1
 * @date 2025-1-16
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef _AK7010_H_
#define _AK7010_H_

#include <stdint.h>
#include <float.h>
#include <math.h>
#include "drivers/AK7010/ak7010_constants.h"
#include "drivers/canbus.h"

// AK70-10 RX/TX Structure
typedef struct {
    // Motor Bus ID
    uint32_t can_id;
    // Tuning Values - Set at compile time
    float    kP, kI, kD, kF;
    // Motor velocity/position rx/tx values
    float    position;
    float    velocity;
    // Motor Temperature (deg C)
    float temp;
    // Motor Current Draw (A)
    float current;
    // Set to true if motor has been enabled
    // bool enable;
    // Error Value (See AK70-10 Datasheet)
    uint8_t error;
} AK7010_t;


// Converts float values into integers (ak7010 compatible conversion)
static uint32_t ak7010_toInt(float x, float x_min, float x_max, int bits){
    /// Converts a float to an unsigned int, given range and number of bits ///
    float span = x_max - x_min;
    if (x < x_min)
        x = x_min;
    else if (x > x_max)
        x = x_max;
    return (uint32_t)((x - x_min) * ((float)((float)((uint32_t)(1UL << bits)) / span)));

}

// Converts integer values into floating point values (ak7010 compatible conversion)
static float ak7010_toFlt(uint32_t x_int, float x_min, float x_max, int bits){
    /// converts unsigned int to float, given range and number of bits ///
    float span = x_max - x_min;
    float offset = x_min;
    return ((float)x_int) * span / ((float)((1 << bits) - 1)) + offset;
}

/**
 * @brief Pack an AK70-10 Control Message into a CAN Message
 *
 * @param mtr AK70-10 Motor Control Message (unmodified)
 * @param msg CAN bus message to pack data into
 */
static inline void ak7010_pack(AK7010_t *mtr, can_msg_t *msg){
    float p_des = fminf(fmaxf(AK_P_MIN, mtr->position), AK_P_MAX);
    float v_des = fminf(fmaxf(AK_V_MIN, mtr->velocity), AK_V_MAX);
    float kp = fminf(fmaxf(AK_KP_MIN, mtr->kP), AK_KP_MAX);
    float kd = fminf(fmaxf(AK_KD_MIN, mtr->kD), AK_KD_MAX);
    float t_ff = fminf(fmaxf(AK_T_MIN, mtr->kF), AK_T_MAX);

    /// convert floats to unsigned ints ///
    uint32_t p_int = ak7010_toInt(p_des, AK_P_MIN, AK_P_MAX, 16);
    uint32_t v_int = ak7010_toInt(v_des, AK_V_MIN, AK_V_MAX, 12);
    uint32_t kp_int = ak7010_toInt(kp, AK_KP_MIN, AK_KP_MAX, 12);
    uint32_t kd_int = ak7010_toInt(kd, AK_KD_MIN, AK_KD_MAX, 12);
    uint32_t t_int = ak7010_toInt(t_ff, AK_T_MIN, AK_T_MAX, 12);

    /// pack ints into the can buffer ///
    msg->id = mtr->can_id;
    msg->len = 8;
    msg->data[0] = (uint8_t)(p_int >> 8U);    // Position 8 higher
    msg->data[1] = (uint8_t)(p_int & 0xFF);  // Position 8 lower
    msg->data[2] = (uint8_t)(v_int >> 4);    // Speed 8 higher
    msg->data[3] = (uint8_t)((v_int & 0xF) << 4) | (uint8_t)(kp_int >> 8);  // Speed 4 bit lower KP 4bit higher
    msg->data[4] = (uint8_t)(kp_int & 0xFF); // KP 8 bit lower
    msg->data[5] = (uint8_t)(kd_int >> 4);            // Kd 8 bit higher
    msg->data[6] = (uint8_t)((kd_int & 0xF) << 4) | (uint8_t)(t_int >> 8);  // KP 4 bit lower torque 4 bit higher;
    msg->data[7] = (uint8_t)(t_int & 0xFF);  // torque 4 bit lower
}

/**
 * @brief Unpack a CAN message into an AK70-10 Motor Control Structure
 *
 * @param mtr Motor message to unpack into
 * @param msg Message from the CAN Bus
 */
static inline void ak7010_unpack(AK7010_t *mtr, can_msg_t *msg){
    /// unpack ints from can buffer ///
    uint32_t p_int = (uint32_t)(msg->data[1] << 8) | msg->data[2];  // Motor position data
    uint32_t v_int = (uint32_t)(msg->data[3] << 4) | (msg->data[4] >> 4);  // Motor speed data
    uint32_t i_int = (uint32_t)((msg->data[4] & 0xF) << 8) | msg->data[5];  // Motor current data
    uint32_t T_int = msg->data[6];

    /// convert ints to floats ///
    mtr->position = ak7010_toFlt(p_int, AK_P_MIN, AK_P_MAX, 16);
    mtr->velocity = ak7010_toFlt(v_int, AK_V_MIN, AK_V_MAX, 12);
    mtr->current = ak7010_toFlt(i_int, -AK_I_MAX, AK_I_MAX, 12);
    mtr->temp = ak7010_toFlt(T_int, AK_Temp_MIN, AK_Temp_MAX, 8);
    mtr->error = msg->data[7];
}

// Retrieve the AK70-10 enable message
static inline void ak7010_enable(AK7010_t *mtr, can_msg_t *msg){
    // ak7010 start code
    msg->data[0] = 0xff;
    msg->data[1] = 0xff;
    msg->data[2] = 0xff;
    msg->data[3] = 0xff;
    msg->data[4] = 0xff;
    msg->data[5] = 0xff;
    msg->data[6] = 0xff;
    msg->data[7] = 0xfc;

    msg->id = mtr->can_id;
    msg->len = 8;
}

// Retrieve the AK70-10 disable message
static inline void ak7010_disable(AK7010_t *mtr, can_msg_t *msg){
    // ak7010 stop code
    msg->data[0] = 0xff;
    msg->data[1] = 0xff;
    msg->data[2] = 0xff;
    msg->data[3] = 0xff;
    msg->data[4] = 0xff;
    msg->data[5] = 0xff;
    msg->data[6] = 0xff;
    msg->data[7] = 0xfd;

    msg->id = mtr->can_id;
    msg->len = 8;
}

// Retrieve the AK70-10 Encoder Zero Message
static inline void ak7010_zero(AK7010_t *mtr, can_msg_t *msg){
    // ak7010 stop code
    msg->data[0] = 0xff;
    msg->data[1] = 0xff;
    msg->data[2] = 0xff;
    msg->data[3] = 0xff;
    msg->data[4] = 0xff;
    msg->data[5] = 0xff;
    msg->data[6] = 0xff;
    msg->data[7] = 0xfe;

    msg->id = mtr->can_id;
    msg->len = 8;
}

#endif  // _AK7010_H_


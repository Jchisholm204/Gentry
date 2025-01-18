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

typedef struct {
    uint32_t can_id;
    float    kP, kI, kD, kF;
    float    position;
    float    velocity;
    float temp;
    float current;
    bool enable;
    uint8_t error;
} AK7010_t;


static uint32_t ak7010_toInt(float x, float x_min, float x_max, int bits){
    /// Converts a float to an unsigned int, given range and number of bits ///
    float span = x_max - x_min;
    if (x < x_min)
        x = x_min;
    else if (x > x_max)
        x = x_max;
    return (int)((x - x_min) * ((float)((1 << bits) / span)));

}

static float ak7010_toFlt(uint32_t x_int, float x_min, float x_max, int bits){
    /// converts unsigned int to float, given range and number of bits ///
    float span = x_max - x_min;
    float offset = x_min;
    return ((float)x_int) * span / ((float)((1 << bits) - 1)) + offset;
}

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
    msg->data[0] = p_int >> 8;    // Position 8 higher
    msg->data[1] = p_int & 0xFF;  // Position 8 lower
    msg->data[2] = v_int >> 4;    // Speed 8 higher
    msg->data[3] =
        ((v_int & 0xF) << 4) | (kp_int >> 8);  // Speed 4 bit lower KP 4bit higher
    msg->data[4] = kp_int & 0xFF;          // KP 8 bit lower
    msg->data[5] = kd_int >> 4;            // Kd 8 bit higher
    msg->data[6] = ((kd_int & 0xF) << 4) |
        (t_int >> 8);  // KP 4 bit lower torque 4 bit higher;
    msg->data[7] = t_int & 0xff;  // torque 4 bit lower
}

static inline void ak7010_unpack(AK7010_t *mtr, can_msg_t *msg){
    /// unpack ints from can buffer ///
    // outGoing.id = msg.data[0];                          // CAN ID of driver?
    uint32_t p_int = (msg->data[1] << 8) | msg->data[2];  // Motor position data
    uint32_t v_int = (msg->data[3] << 4) | (msg->data[4] >> 4);  // Motor speed data
    uint32_t i_int =
        ((msg->data[4] & 0xF) << 8) | msg->data[5];  // Motor current data
    uint32_t T_int = msg->data[6];

    /// convert ints to floats ///
    mtr->position = ak7010_toFlt(p_int, P_MIN, P_MAX, 16);
    mtr->velocity = ak7010_toFlt(v_int, V_MIN, V_MAX, 12);
    mtr->current = ak7010_toFlt(i_int, -I_MAX, I_MAX, 12);
    mtr->temp = ak7010_toFlt(T_int, Temp_MIN, Temp_MAX, 8);
    mtr->error = msg->data[7];
}

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

#endif  // _AK7010_H_


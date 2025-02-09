/**
 * @file AKMOTOR.h
 * @author Jacob Chisholm (https://Jchisholm204.github.io)
 * @brief Generic Structure for controlling an AK Motor
 * @version 0.1
 * @date 2025-1-16
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef _AKMOTOR_H_
#define _AKMOTOR_H_


#include <stdint.h>
#include <math.h>
#include "AKMotor_constants.h"
#include "hal/hal_can.h"

// AK70-10 RX/TX Structure
typedef struct {
    // Motor Bus ID
    const uint32_t can_id;
    // Motor Type (Model Number)
    const enum AKMotorType type;
    // Tuning Values
    float    kP, kI, kD, kF;
    // Motor velocity/position rx/tx values
    float    position;
    float    velocity;
    // Motor Temperature (deg C)
    float temp;
    // Motor Current Draw (A)
    float current;
    // Set to true if motor has been enabled
    bool enable;
    // Error Value (See AK70-10 Datasheet)
    uint8_t error;
} AkMotor_t;

// Converts float values into integers (ak7010 compatible conversion)
static uint32_t akMotor_toInt(float x, float x_min, float x_max, int bits){
    /// Converts a float to an unsigned int, given range and number of bits ///
    float span = x_max - x_min;
    if (x < x_min)
        x = x_min;
    else if (x > x_max)
        x = x_max;
    return (uint32_t)((x - x_min) * ((float)((float)((uint32_t)(1UL << bits)) / span)));

}

// Converts integer values into floating point values (ak7010 compatible conversion)
static float akMotor_toFlt(uint32_t x_int, float x_min, float x_max, int bits){
    /// converts unsigned int to float, given range and number of bits ///
    float span = x_max - x_min;
    float offset = x_min;
    return ((float)x_int) * span / ((float)((1 << bits) - 1)) + offset;
}

// confines a value to the minimum and maximum value
static inline float akMotor_minmax(float val, float max, float min){
    if(val < min) val = min;
    if(val > max) val = max;
    return val;
}

/**
 * @brief Pack an AK70-10 Control Message into a CAN Message
 *
 * @param mtr AK70-10 Motor Control Message (unmodified)
 * @param msg CAN bus message to pack data into
 */
static inline void akMotor_pack(AkMotor_t *mtr, can_msg_t *msg){
    const struct AKMotorConfig *pCfg = &AKConfigs[mtr->type];
    float p_des = akMotor_minmax(mtr->position, pCfg->pos_min, pCfg->pos_max);
    float v_des = akMotor_minmax(mtr->velocity, pCfg->vel_min, pCfg->vel_max);
    float kp = akMotor_minmax(mtr->kP, pCfg->kp_min, pCfg->kp_max);
    float kd = akMotor_minmax(mtr->kD, pCfg->kd_min, pCfg->kd_max);
    float t_ff = akMotor_minmax(mtr->kF, pCfg->trq_min, pCfg->trq_min);

    /// convert floats to unsigned ints ///
    uint32_t p_int = akMotor_toInt(p_des, pCfg->pos_min, pCfg->pos_max, 16);
    uint32_t v_int = akMotor_toInt(v_des, pCfg->vel_min, pCfg->vel_max, 12);
    uint32_t kp_int = akMotor_toInt(kp, pCfg->kp_min, pCfg->kp_max, 12);
    uint32_t kd_int = akMotor_toInt(kd, pCfg->kd_min, pCfg->kd_max, 12);
    uint32_t t_int = akMotor_toInt(t_ff, pCfg->trq_min, pCfg->trq_max, 12);

    /// pack ints into the can buffer ///
    msg->id = mtr->can_id;
    msg->len = 8;
    msg->data[0] = (uint8_t)((p_int >> 8U) & 0xFFU);    // Position 8 higher
    msg->data[1] = (uint8_t)(p_int & 0xFFU);  // Position 8 lower
    msg->data[2] = (uint8_t)((v_int >> 4) & 0xFF);    // Speed 8 higher
    msg->data[3] = (uint8_t)((uint8_t)((v_int << 4) & 0xF0U) | (uint8_t)((uint8_t)(kp_int >> 8) & 0x0FU));  // Speed 4 bit lower KP 4bit higher
    msg->data[4] = (uint8_t)(kp_int & 0xFFU); // KP 8 bit lower
    msg->data[5] = (uint8_t)(kd_int >> 4);            // Kd 8 bit higher
    msg->data[6] = (uint8_t)((uint8_t)((kd_int & 0xFU) << 4) | ((uint8_t)(t_int >> 8) & 0x0FU));  // KP 4 bit lower torque 4 bit higher;
    msg->data[7] = (uint8_t)(t_int & 0xFFU);  // torque 4 bit lower
}

/**
 * @brief Unpack a CAN message into an AK70-10 Motor Control Structure
 *
 * @param mtr Motor message to unpack into
 * @param msg Message from the CAN Bus
 */
static inline void akMotor_unpack(AkMotor_t *mtr, can_msg_t *msg){
    const struct AKMotorConfig *pCfg = &AKConfigs[mtr->type];
    /// unpack ints from can buffer ///
    uint32_t p_int = (uint32_t)(msg->data[1] << 8) | msg->data[2];  // Motor position data
    uint32_t v_int = (uint32_t)(msg->data[3] << 4) | (msg->data[4] >> 4);  // Motor speed data
    uint32_t i_int = (uint32_t)((msg->data[4] & 0xF) << 8) | msg->data[5];  // Motor current data
    uint32_t T_int = msg->data[6];

    /// convert ints to floats ///
    mtr->position = akMotor_toFlt(p_int, pCfg->pos_min, pCfg->pos_max, 16);
    mtr->velocity = akMotor_toFlt(v_int, pCfg->vel_min, pCfg->vel_max, 12);
    mtr->current = akMotor_toFlt(i_int, pCfg->cur_min, pCfg->cur_max, 12);
    mtr->temp = akMotor_toFlt(T_int, pCfg->tmp_min, pCfg->tmp_max, 8);
    mtr->error = msg->data[7];
}

// Retrieve the AK70-10 enable message
static inline void akMotor_enable(AkMotor_t *mtr, can_msg_t *msg){
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
static inline void akMotor_disable(AkMotor_t *mtr, can_msg_t *msg){
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
static inline void akMotor_zero(AkMotor_t *mtr, can_msg_t *msg){
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

#endif

/**
 * @file TMC2208.c
 * @author Jacob Chisholm (https://Jchisholm204.github.io)
 * @brief TMC2208 Stepper Motor Drivers
 * @version 0.1
 * @date Created: 2025-04-03
 * @modified Last Modified: 2025-04-22
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "devices/TMC2208.h"
#include "devices/TMC2208_defs.h"

// TMC 22xx CRC8-ATM Calculator
uint8_t crc(uint8_t *data, uint8_t len){
    uint8_t crc = 0;
    for(int i = 0; i < len; i++){
        uint8_t byte = data[i];
        for(int j = 0; j < 8; j++){
            if((crc >> 7) ^ (byte & 0x01))
                crc = ((crc >> 1) ^ 0x07) & 0xFF;
            else
                crc = (crc << 1) & 0xFF;
            byte = byte >> 1;
        }
    }
    return crc;
}

eTMC2208Error write_reg(TMC2208_t *pDev, uint8_t reg, uint32_t data){
    struct tmc2208_datagram datagram;
    datagram.sync = TMC2208_SYNC;
    datagram.slave_addr = TMC2208_SLAVE_ADDR;
    datagram.reg_addr = reg | TMC2208_WRITE;
    datagram.data = data;
    datagram.crc = crc((void*)&datagram, sizeof(struct tmc2208_datagram)-1);
    serial_write(pDev->pSerial, (char*)&datagram, sizeof(struct tmc2208_datagram), 100);
    return eTMC2208_OK;
}

int read_reg(TMC2208_t *pDev, uint8_t reg, struct tmc2208_datagram *datagram){
    uint8_t pkt_req[] = {TMC2208_SYNC, TMC2208_SLAVE_ADDR, reg, 0x00};
    pkt_req[3] = crc(pkt_req, 3);
    
    serial_write(pDev->pSerial, (char*)pkt_req, 3, 100);
    xStreamBufferReset(pDev->buffer_hndl);
    size_t rx_bytes = xStreamBufferReceive(pDev->buffer_hndl, datagram, sizeof(struct tmc2208_datagram), 1000);
    if(rx_bytes < sizeof(struct tmc2208_datagram)){
        return -1;
    }
    if(datagram->crc != crc((void*)datagram, sizeof(struct tmc2208_datagram)-1))
        return -2;
    if(datagram->slave_addr != 0xFF)
        return -3;
    return 0;
}

eTMC2208Error tmc2208_init(TMC2208_t *pDev, Serial_t *pSerial){
    if(!pDev || !pSerial) return eTMC2208_NullParam;
    if(pSerial->state == eSerialNoInit) return eTMC2208_NoSerial;
    pDev->pSerial = pSerial;
    pDev->pMaster = NULL;
    pDev->pSlave = NULL;
    write_reg(NULL, 0, 0);
    return eTMC2208_OK;
}

eTMC2208Error tmc2208_initChain(TMC2208_t * const pMaster, TMC2208_t * const pSlave, Serial_t *pSerial){
    if(!pMaster || pSlave || !pSerial) return eTMC2208_NullParam;
    // Setup the Slave
    tmc2208_init(pSlave, pSerial);
    // Set the slave to have a master
    pSlave->pMaster = pMaster;
    // Set the master to have a slave
    pMaster->pSlave = pSlave;
    return eTMC2208_OK;
}

eTMC2208Error tmc2208_setMotion(TMC2208_t *pDev, int32_t speed, uint32_t steps){
    (void)pDev;
    (void)speed;
    (void)steps;
    return eTMC2208_OK;
}

/*	
	Requested current = mA = I_rms/1000
	Equation for current:
	I_rms = (CS+1)/32 * V_fs/(R_sense+0.02ohm) * 1/sqrt(2)
	Solve for CS ->
	CS = 32*sqrt(2)*I_rms*(R_sense+0.02)/V_fs - 1
	
	Example:
	vsense = 0b0 -> V_fs = 0.325V
	mA = 1640mA = I_rms/1000 = 1.64A
	R_sense = 0.10 Ohm
	->
	CS = 32*sqrt(2)*1.64*(0.10+0.02)/0.325 - 1 = 26.4
	CS = 26
*/
eTMC2208Error tmc2208_setCurrent(TMC2208_t *pDev, uint8_t hold, uint8_t run){
    return eTMC2208_OK;
}

eTMC2208Error tmc2208_setHoldDelay(TMC2208_t *pDev, uint8_t hold_delay){
    (void)pDev;
    (void)hold_delay;
    return eTMC2208_OK;
}

eTMC2208Error tmc2208_getVelocity(TMC2208_t *pDev, uint32_t *speed){
    (void)pDev;
    (void)speed;
    return eTMC2208_OK;
}

eTMC2208Error tmc2208_getCurrent(TMC2208_t *pDev, int16_t *current){
    (void)pDev;
    (void)current;
    return eTMC2208_OK;
}


/*********************************************************************
*                     SEGGER Microcontroller GmbH                    *
*                        The Embedded Experts                        *
**********************************************************************
*                                                                    *
*       (c) 1995 - 2022 SEGGER Microcontroller GmbH                  *
*                                                                    *
*       Internet: segger.com  Support: support_embos@segger.com      *
*                                                                    *
**********************************************************************
*                                                                    *
*       embOS * Real time operating system                           *
*                                                                    *
*       Please note:                                                 *
*                                                                    *
*       Knowledge of this file may under no circumstances            *
*       be used to write a similar product or a real-time            *
*       operating system for in-house use.                           *
*                                                                    *
*       Thank you for your fairness !                                *
*                                                                    *
**********************************************************************
*                                                                    *
*       OS version: V5.18.0.0                                        *
*                                                                    *
**********************************************************************

--------- END-OF-HEADER ---------------------------------*/

/****** FPGA Base Address (defined in Xilinx Vivado Tool) *************/
#define HW_FPGA_REG_BASE_ADDR 0x43C00000

/****** MIO, EMIO Configuration ********************************************/

#define MIO_BASE_ADDR           0xE000A000
#define REG_MIO_BANK0_DATA_RO   *(volatile U32*)(MIO_BASE_ADDR + 0x0060)  // Input data
#define REG_MIO_MASK_DATA_0_LSW *(volatile U32*)(MIO_BASE_ADDR + 0x0000) // Maskable Output Data (GPIO Bank0, MIO, Lower 16bits)
#define REG_MIO_MASK_DATA_0_MSW *(volatile U32*)(MIO_BASE_ADDR + 0x0004) // Maskable Output Data (GPIO Bank0, MIO, Upper 16bits)
#define REG_MIO_MASK_DATA_1_LSW *(volatile U32*)(MIO_BASE_ADDR + 0x0008) // Maskable Output Data (GPIO Bank1, MIO, Lower 16bits)
#define REG_MIO_MASK_DATA_1_MSW *(volatile U32*)(MIO_BASE_ADDR + 0x000C) // Maskable Output Data (GPIO Bank1, MIO, Upper 6bits)
#define REG_MIO_MASK_DATA_2_LSW *(volatile U32*)(MIO_BASE_ADDR + 0x0010) // Maskable Output Data (GPIO Bank2, EMIO, Lower 16bits)
#define REG_MIO_MASK_DATA_2_MSW *(volatile U32*)(MIO_BASE_ADDR + 0x0014) // Maskable Output Data (GPIO Bank2, EMIO, Upper 16bits)
#define REG_MIO_MASK_DATA_3_LSW *(volatile U32*)(MIO_BASE_ADDR + 0x0018) // Maskable Output Data (GPIO Bank3, EMIO, Lower 16bits)
#define REG_MIO_MASK_DATA_3_MSW *(volatile U32*)(MIO_BASE_ADDR + 0x001C) // Maskable Output Data (GPIO Bank3, EMIO, Upper 16bits)
#define REG_MIO_DATA_0          *(volatile U32*)(MIO_BASE_ADDR + 0x0040) // Output Data (GPIO Bank0, MIO)
#define REG_MIO_DATA_1          *(volatile U32*)(MIO_BASE_ADDR + 0x0044) // Output Data (GPIO Bank1, MIO)
#define REG_MIO_DATA_2          *(volatile U32*)(MIO_BASE_ADDR + 0x0048) // Output Data (GPIO Bank2, EMIO)
#define REG_MIO_DATA_3          *(volatile U32*)(MIO_BASE_ADDR + 0x004C) // Output Data (GPIO Bank3, EMIO)
#define REG_MIO_DATA_0_RO       *(volatile U32*)(MIO_BASE_ADDR + 0x0060) // Input Data (GPIO Bank0, MIO)
#define REG_MIO_DATA_1_RO       *(volatile U32*)(MIO_BASE_ADDR + 0x0064) // Input Data (GPIO Bank1, MIO)
#define REG_MIO_DATA_2_RO       *(volatile U32*)(MIO_BASE_ADDR + 0x0068) // Input Data (GPIO Bank2, EMIO)
#define REG_MIO_DATA_3_RO       *(volatile U32*)(MIO_BASE_ADDR + 0x006C) // Input Data (GPIO Bank3, EMIO)
#define REG_MIO_DIRM_0          *(volatile U32*)(MIO_BASE_ADDR + 0x0204) // Direction mode (GPIO Bank0, MIO)
#define REG_MIO_OEN_0           *(volatile U32*)(MIO_BASE_ADDR + 0x0208) // Output enable (GPIO Bank0, MIO)
#define REG_MIO_INT_MASK_0      *(volatile U32*)(MIO_BASE_ADDR + 0x020C) // Interrupt Mask Status (GPIO Bank0, MIO)
#define REG_MIO_INT_EN_0        *(volatile U32*)(MIO_BASE_ADDR + 0x0210) // Interrupt Enable/Unmask (GPIO Bank0, MIO)
#define REG_MIO_INT_DIS_0       *(volatile U32*)(MIO_BASE_ADDR + 0x0214) // Interrupt Disable/Mask (GPIO Bank0, MIO)
#define REG_MIO_INT_STAT_0      *(volatile U32*)(MIO_BASE_ADDR + 0x0218) // Interrupt Status (GPIO Bank0, MIO)
#define REG_MIO_INT_TYPE_0      *(volatile U32*)(MIO_BASE_ADDR + 0x021C) // Interrupt Type (GPIO Bank0, MIO)
#define REG_MIO_INT_POLARITY_0  *(volatile U32*)(MIO_BASE_ADDR + 0x0220) // Interrupt Polarity (GPIO Bank0, MIO)
#define REG_MIO_INT_ANY_0       *(volatile U32*)(MIO_BASE_ADDR + 0x0224) // Interrupt Any Edge Sensitive (GPIO Bank0, MIO)
#define REG_MIO_DIRM_1          *(volatile U32*)(MIO_BASE_ADDR + 0x0244) // Direction mode (GPIO Bank1, MIO)
#define REG_MIO_OEN_1           *(volatile U32*)(MIO_BASE_ADDR + 0x0248) // Output enable (GPIO Bank1, MIO)
#define REG_MIO_INT_MASK_1      *(volatile U32*)(MIO_BASE_ADDR + 0x024C) // Interrupt Mask Status (GPIO Bank1, MIO)
#define REG_MIO_INT_EN_1        *(volatile U32*)(MIO_BASE_ADDR + 0x0250) // Interrupt Enable/Unmask (GPIO Bank1, MIO)
#define REG_MIO_INT_DIS_1       *(volatile U32*)(MIO_BASE_ADDR + 0x0254) // Interrupt Disable/Mask (GPIO Bank1, MIO)
#define REG_MIO_INT_STAT_1      *(volatile U32*)(MIO_BASE_ADDR + 0x0258) // Interrupt Status (GPIO Bank1, MIO)
#define REG_MIO_INT_TYPE_1      *(volatile U32*)(MIO_BASE_ADDR + 0x025C) // Interrupt Type (GPIO Bank1, MIO)
#define REG_MIO_INT_POLARITY_1  *(volatile U32*)(MIO_BASE_ADDR + 0x0260) // Interrupt Polarity (GPIO Bank1, MIO)
#define REG_MIO_INT_ANY_1       *(volatile U32*)(MIO_BASE_ADDR + 0x0264) // Interrupt Any Edge Sensitive (GPIO Bank1, MIO)
#define REG_MIO_DIRM_2          *(volatile U32*)(MIO_BASE_ADDR + 0x0284) // Direction mode (GPIO Bank2, EMIO)
#define REG_MIO_OEN_2           *(volatile U32*)(MIO_BASE_ADDR + 0x0288) // Output enable (GPIO Bank2, EMIO)
#define REG_MIO_INT_MASK_2      *(volatile U32*)(MIO_BASE_ADDR + 0x028C) // Interrupt Mask Status (GPIO Bank2, EMIO)
#define REG_MIO_INT_EN_2        *(volatile U32*)(MIO_BASE_ADDR + 0x0290) // Interrupt Enable/Unmask (GPIO Bank2, EMIO)
#define REG_MIO_INT_DIS_2       *(volatile U32*)(MIO_BASE_ADDR + 0x0294) // Interrupt Disable/Mask (GPIO Bank2, EMIO)
#define REG_MIO_INT_STAT_2      *(volatile U32*)(MIO_BASE_ADDR + 0x0298) // Interrupt Status (GPIO Bank2, EMIO)
#define REG_MIO_INT_TYPE_2      *(volatile U32*)(MIO_BASE_ADDR + 0x029C) // Interrupt Type (GPIO Bank2, EMIO)
#define REG_MIO_INT_POLARITY_2  *(volatile U32*)(MIO_BASE_ADDR + 0x02A0) // Interrupt Polarity (GPIO Bank2, EMIO)
#define REG_MIO_INT_ANY_2       *(volatile U32*)(MIO_BASE_ADDR + 0x02A4) // Interrupt Any Edge Sensitive (GPIO Bank2, EMIO)
#define REG_MIO_DIRM_3          *(volatile U32*)(MIO_BASE_ADDR + 0x02C4) // Direction mode (GPIO Bank3, EMIO)
#define REG_MIO_OEN_3           *(volatile U32*)(MIO_BASE_ADDR + 0x02C8) // Output enable (GPIO Bank3, EMIO)
#define REG_MIO_INT_MASK_3      *(volatile U32*)(MIO_BASE_ADDR + 0x02CC) // Interrupt Mask Status (GPIO Bank3, EMIO)
#define REG_MIO_INT_EN_3        *(volatile U32*)(MIO_BASE_ADDR + 0x02D0) // Interrupt Enable/Unmask (GPIO Bank3, EMIO)
#define REG_MIO_INT_DIS_3       *(volatile U32*)(MIO_BASE_ADDR + 0x02D4) // Interrupt Disable/Mask (GPIO Bank3, EMIO)
#define REG_MIO_INT_STAT_3      *(volatile U32*)(MIO_BASE_ADDR + 0x02D8) // Interrupt Status (GPIO Bank3, EMIO)
#define REG_MIO_INT_TYPE_3      *(volatile U32*)(MIO_BASE_ADDR + 0x02DC) // Interrupt Type (GPIO Bank3, EMIO)
#define REG_MIO_INT_POLARITY_3  *(volatile U32*)(MIO_BASE_ADDR + 0x02E0) // Interrupt Polarity (GPIO Bank3, EMIO)
#define REG_MIO_INT_ANY_3       *(volatile U32*)(MIO_BASE_ADDR + 0x02E4) // Interrupt Any Edge Sensitive (GPIO Bank3, EMIO)

/****** DDR Controller ********************************************/
#define DDRC_BASE_ADDR (0xF8006000)
#define REG_DDRC_CTRL                                   *(volatile U32*)(0xF8006000) // DDRC Control
#define REG_DDRC_TWO_RANK_CFG                           *(volatile U32*)(0xF8006004) // Two Rank Configuration
#define REG_DDRC_HPR_REG                                *(volatile U32*)(0xF8006008) // HPR Queue control
#define REG_DDRC_LPR_REG                                *(volatile U32*)(0xF800600C) // LPR Queue control
#define REG_DDRC_WR_REG                                 *(volatile U32*)(0xF8006010) // WR Queue control
#define REG_DDRC_DRAM_PARAM_REG0                        *(volatile U32*)(0xF8006014) // DRAM Parameters 0
#define REG_DDRC_DRAM_PARAM_REG1                        *(volatile U32*)(0xF8006018) // DRAM Parameters 1
#define REG_DDRC_DRAM_PARAM_REG2                        *(volatile U32*)(0xF800601C) // DRAM Parameters 2
#define REG_DDRC_DRAM_PARAM_REG3                        *(volatile U32*)(0xF8006020) // DRAM Parameters 3
#define REG_DDRC_DRAM_PARAM_REG4                        *(volatile U32*)(0xF8006024) // DRAM Parameters 4
#define REG_DDRC_DRAM_INIT_PARAM                        *(volatile U32*)(0xF8006028) // DRAM Initialization Parameters
#define REG_DDRC_DRAM_EMR_REG                           *(volatile U32*)(0xF800602C) // DRAM EMR2, EMR3 access
#define REG_DDRC_DRAM_EMR_MR_REG                        *(volatile U32*)(0xF8006030) // DRAM EMR, MR access
#define REG_DDRC_DRAM_BURST8_RDWR                       *(volatile U32*)(0xF8006034) // DRAM Burst 8 read/write
#define REG_DDRC_DRAM_DISABLE_DQ                        *(volatile U32*)(0xF8006038) // DRAM Disable DQDRAM_addr_map_bank
#define REG_DDRC_DRAM_ADDR_MAP_BANK                     *(volatile U32*)(0xF800603C) // Row/Column address bits
#define REG_DDRC_DRAM_ADDR_MAP_COL                      *(volatile U32*)(0xF8006040) // Column address bits
#define REG_DDRC_DRAM_ADDR_MAP_ROW                      *(volatile U32*)(0xF8006044) // Select DRAM row address bits
#define REG_DDRC_DRAM_ODT_REG                           *(volatile U32*)(0xF8006048) // DRAM ODT control
#define REG_DDRC_PHY_DBG_REG                            *(volatile U32*)(0xF800604C) // PHY debug
#define REG_DDRC_PHY_CMD_TIMEOUT_RDD_ATA_CPT            *(volatile U32*)(0xF8006050) // PHY command time out and read data capture FIFO
#define REG_DDRC_MODE_STS_REG                           *(volatile U32*)(0xF8006054) // Controller operation mode status
#define REG_DDRC_DLL_CALIB                              *(volatile U32*)(0xF8006058) // DLL calibration
#define REG_DDRC_ODT_DELAY_HOLD                         *(volatile U32*)(0xF800605C) // ODT delay and ODT hold
#define REG_DDRC_CTRL_REG1                              *(volatile U32*)(0xF8006060) // Controller 1
#define REG_DDRC_CTRL_REG2                              *(volatile U32*)(0xF8006064) // Controller 2
#define REG_DDRC_CTRL_REG3                              *(volatile U32*)(0xF8006068) // Controller 3
#define REG_DDRC_CTRL_REG4                              *(volatile U32*)(0xF800606C) // Controller 4
#define REG_DDRC_CTRL_REG5                              *(volatile U32*)(0xF8006078) // Controller register 5
#define REG_DDRC_CTRL_REG6                              *(volatile U32*)(0xF800607C) // Controller register 6
#define REG_DDRC_CHE_REFRESH_TIMER01                    *(volatile U32*)(0xF80060A0) // CHE_REFRESH_TIMER01
#define REG_DDRC_CHE_T_ZQ                               *(volatile U32*)(0xF80060A4) // ZQ parameters
#define REG_DDRC_CHE_T_ZQ_SHORT_INTERVAL_REG            *(volatile U32*)(0xF80060A8) // Misc parameters
#define REG_DDRC_DEEP_PWRDWN_REG                        *(volatile U32*)(0xF80060AC) // Deep powerdown (LPDDR2)
#define REG_DDRC_REG_2C                                 *(volatile U32*)(0xF80060B0) // Training control
#define REG_DDRC_REG_2D                                 *(volatile U32*)(0xF80060B4) // Misc Debug
#define REG_DDRC_DFI_TIMING                             *(volatile U32*)(0xF80060B8) // DFI timing
#define REG_DDRC_CHE_ECC_CONTROL_REG_OFFSET             *(volatile U32*)(0xF80060C4) // ECC error clear
#define REG_DDRC_CHE_CORR_ECC_LOG_REG_OFFSET            *(volatile U32*)(0xF80060C8) // ECC error correction
#define REG_DDRC_CHE_CORR_ECC_ADDR_REG_OFFSET           *(volatile U32*)(0xF80060CC) // ECC error correction address log
#define REG_DDRC_CHE_CORR_ECC_DATA_31_0_REG_OFFSET      *(volatile U32*)(0xF80060D0) // ECC error correction data log low
#define REG_DDRC_CHE_CORR_ECC_DATA_63_32_REG_OFFSET     *(volatile U32*)(0xF80060D4) // ECC error correction data log mid
#define REG_DDRC_CHE_CORR_ECC_DATA_71_64_REG_OFFSET     *(volatile U32*)(0xF80060D8) // ECC error correction data log high
#define REG_DDRC_CHE_UNCORR_ECC_LOG_REG_OFFSET          *(volatile U32*)(0xF80060DC) // ECC unrecoverable error status
#define REG_DDRC_CHE_UNCORR_ECC_ADDR_REG_OFFSET         *(volatile U32*)(0xF80060E0) // ECC unrecoverable error address
#define REG_DDRC_CHE_UNCORR_ECC_DATA_31_0_REG_OFFSET    *(volatile U32*)(0xF80060E4) // ECC unrecoverable error data low
#define REG_DDRC_CHE_UNCORR_ECC_DATA_63_32_REG_OFFSET   *(volatile U32*)(0xF80060E8) // ECC unrecoverable error data middle
#define REG_DDRC_CHE_UNCORR_ECC_DATA_71_64_REG_OFFSET   *(volatile U32*)(0xF80060EC) // ECC unrecoverable error data high
#define REG_DDRC_CHE_ECC_STATS_REG_OFFSET               *(volatile U32*)(0xF80060F0) // ECC error count
#define REG_DDRC_ECC_SCRUB                              *(volatile U32*)(0xF80060F4) // ECC mode/scrub
#define REG_DDRC_CHE_ECC_CORR_BIT_MASK_31_0_REG_OFFSET  *(volatile U32*)(0xF80060F8) // ECC data mask low
#define REG_DDRC_CHE_ECC_CORR_BIT_MASK_63_32_REG_OFFSET *(volatile U32*)(0xF80060FC) // ECC data mask high
#define REG_DDRC_PHY_RCVR_ENABLE                        *(volatile U32*)(0xF8006114) // Phy receiver enable register
#define REG_DDRC_PHY_CONFIG0                            *(volatile U32*)(0xF8006118) // PHY configuration register for data slice 0.
#define REG_DDRC_PHY_CONFIG1                            *(volatile U32*)(0xF800611C) // PHY configuration register for data slice 1.
#define REG_DDRC_PHY_CONFIG2                            *(volatile U32*)(0xF8006120) // PHY configuration register for data slice 2.
#define REG_DDRC_PHY_CONFIG3                            *(volatile U32*)(0xF8006124) // PHY configuration register for data slice 3.
#define REG_DDRC_PHY_INIT_RATIO0                        *(volatile U32*)(0xF800612C) // PHY init ratio register for data slice 0.
#define REG_DDRC_PHY_INIT_RATIO1                        *(volatile U32*)(0xF8006130) // PHY init ratio register for data slice 1.
#define REG_DDRC_PHY_INIT_RATIO2                        *(volatile U32*)(0xF8006134) // PHY init ratio register for data slice 2.
#define REG_DDRC_PHY_INIT_RATIO3                        *(volatile U32*)(0xF8006138) // PHY init ratio register for data slice 3.
#define REG_DDRC_PHY_RD_DQS_CFG0                        *(volatile U32*)(0xF8006140) // PHY read DQS configuration register for data slice 0.
#define REG_DDRC_PHY_RD_DQS_CFG1                        *(volatile U32*)(0xF8006144) // PHY read DQS configuration register for data slice 1.
#define REG_DDRC_PHY_RD_DQS_CFG2                        *(volatile U32*)(0xF8006148) // PHY read DQS configuration register for data slice 2.
#define REG_DDRC_PHY_RD_DQS_CFG3                        *(volatile U32*)(0xF800614C) // PHY read DQS configuration register for data slice 3.
#define REG_DDRC_PHY_WR_DQS_CFG0                        *(volatile U32*)(0xF8006154) // PHY write DQS configuration register for data slice 0.
#define REG_DDRC_PHY_WR_DQS_CFG1                        *(volatile U32*)(0xF8006158) // PHY write DQS configuration register for data slice 1.
#define REG_DDRC_PHY_WR_DQS_CFG2                        *(volatile U32*)(0xF800615C) // PHY write DQS configuration register for data slice 2.
#define REG_DDRC_PHY_WR_DQS_CFG3                        *(volatile U32*)(0xF8006160) // PHY write DQS configuration register for data slice 3.
#define REG_DDRC_PHY_WE_CFG0                            *(volatile U32*)(0xF8006168) // PHY FIFO write enable configuration for data slice 0.
#define REG_DDRC_PHY_WE_CFG1                            *(volatile U32*)(0xF800616C) // PHY FIFO write enable configuration for data slice 1.
#define REG_DDRC_PHY_WE_CFG2                            *(volatile U32*)(0xF8006170) // PHY FIFO write enable configuration for data slice 2.
#define REG_DDRC_PHY_WE_CFG3                            *(volatile U32*)(0xF8006174) // PHY FIFO write enable configuration for data slice 3.
#define REG_DDRC_WR_DATA_SLV0                           *(volatile U32*)(0xF800617C) // PHY write data slave ratio config for data slice 0.
#define REG_DDRC_WR_DATA_SLV1                           *(volatile U32*)(0xF8006180) // PHY write data slave ratio config for data slice 1.
#define REG_DDRC_WR_DATA_SLV2                           *(volatile U32*)(0xF8006184) // PHY write data slave ratio config for data slice 2.
#define REG_DDRC_WR_DATA_SLV3                           *(volatile U32*)(0xF8006188) // PHY write data slave ratio config for data slice 3.
#define REG_DDRC_REG_64                                 *(volatile U32*)(0xF8006190) // Training control 2
#define REG_DDRC_REG_65                                 *(volatile U32*)(0xF8006194) // Training control 3
#define REG_DDRC_REG69_6A0                              *(volatile U32*)(0xF80061A4) // Training results for data slice 0.
#define REG_DDRC_REG69_6A1                              *(volatile U32*)(0xF80061A8) // Training results for data slice 1.
#define REG_DDRC_REG6C_6D2                              *(volatile U32*)(0xF80061B0) // Training results for data slice 2.
#define REG_DDRC_REG6C_6D3                              *(volatile U32*)(0xF80061B4) // Training results for data slice 3.
#define REG_DDRC_REG6E_710                              *(volatile U32*)(0xF80061B8) // Training results (2) for data slice 0.
#define REG_DDRC_REG6E_711                              *(volatile U32*)(0xF80061BC) // Training results (2) for data slice 1.
#define REG_DDRC_REG6E_712                              *(volatile U32*)(0xF80061C0) // Training results (2) for data slice 2.
#define REG_DDRC_REG6E_713                              *(volatile U32*)(0xF80061C4) // Training results (2) for data slice 3.
#define REG_DDRC_PHY_DLL_STS0                           *(volatile U32*)(0xF80061CC) // Slave DLL results for data slice 0.
#define REG_DDRC_PHY_DLL_STS1                           *(volatile U32*)(0xF80061D0) // Slave DLL results for data slice 1.
#define REG_DDRC_PHY_DLL_STS2                           *(volatile U32*)(0xF80061D4) // Slave DLL results for data slice 2.
#define REG_DDRC_PHY_DLL_STS3                           *(volatile U32*)(0xF80061D8) // Slave DLL results for data slice 3.
#define REG_DDRC_DLL_LOCK_STS                           *(volatile U32*)(0xF80061E0) // DLL Lock Status, read
#define REG_DDRC_PHY_CTRL_STS                           *(volatile U32*)(0xF80061E4) // PHY Control status, read
#define REG_DDRC_PHY_CTRL_STS_REG2                      *(volatile U32*)(0xF80061E8) // PHY Control status (2), read
#define REG_DDRC_AXI_ID                                 *(volatile U32*)(0xF8006200) // ID and revision information
#define REG_DDRC_PAGE_MASK                              *(volatile U32*)(0xF8006204) // Page mask
#define REG_DDRC_AXI_PRIORITY_WR_PORT0                  *(volatile U32*)(0xF8006208) // AXI Priority control for write port 0.
#define REG_DDRC_AXI_PRIORITY_WR_PORT1                  *(volatile U32*)(0xF800620C) // AXI Priority control for write port 1.
#define REG_DDRC_AXI_PRIORITY_WR_PORT2                  *(volatile U32*)(0xF8006210) // AXI Priority control for write port 2.
#define REG_DDRC_AXI_PRIORITY_WR_PORT3                  *(volatile U32*)(0xF8006214) // AXI Priority control for write port 3.
#define REG_DDRC_AXI_PRIORITY_RD_PORT0                  *(volatile U32*)(0xF8006218) // AXI Priority control for read port 0.
#define REG_DDRC_AXI_PRIORITY_RD_PORT1                  *(volatile U32*)(0xF800621C) // AXI Priority control for read port 1.
#define REG_DDRC_AXI_PRIORITY_RD_PORT2                  *(volatile U32*)(0xF8006220) // AXI Priority control for read port 2.
#define REG_DDRC_AXI_PRIORITY_RD_PORT3                  *(volatile U32*)(0xF8006224) // AXI Priority control for read port 3.
#define REG_DDRC_EXCL_ACCESS_CFG0                       *(volatile U32*)(0xF8006294) // Exclusive access configuration for port 0.
#define REG_DDRC_EXCL_ACCESS_CFG1                       *(volatile U32*)(0xF8006298) // Exclusive access configuration for port 1.
#define REG_DDRC_EXCL_ACCESS_CFG2                       *(volatile U32*)(0xF800629C) // Exclusive access configuration for port 2.
#define REG_DDRC_EXCL_ACCESS_CFG3                       *(volatile U32*)(0xF80062A0) // Exclusive access configuration for port 3.
#define REG_DDRC_MODE_REG_READ                          *(volatile U32*)(0xF80062A4) // Mode register read data
#define REG_DDRC_LPDDR_CTRL0                            *(volatile U32*)(0xF80062A8) // LPDDR2 Control 0
#define REG_DDRC_LPDDR_CTRL1                            *(volatile U32*)(0xF80062AC) // LPDDR2 Control 1
#define REG_DDRC_LPDDR_CTRL2                            *(volatile U32*)(0xF80062B0) // LPDDR2 Control 2
#define REG_DDRC_LPDDR_CTRL3                            *(volatile U32*)(0xF80062B4) // LPDDR2 Control 3

/****** System Level Control ********************************************/

#define MIO_PIN_OPT_DIS_RECV        (1 << 13)
#define MIO_PIN_OPT_ENA_PULLUP      (1 << 12)
#define MIO_PIN_OPT_IO_CMOS_3V3     (3 <<  9)
#define MIO_PIN_OPT_IO_CMOS_2V5     (2 <<  9)
#define MIO_PIN_OPT_IO_CMOS_1V8     (1 <<  9)
#define MIO_PIN_OPT_IO_HSTL         (4 <<  9)
#define MIO_PIN_OPT_ENA_SPEED_FAST  (1 <<  8)
#define MIO_PIN_OPT_LVL30_MUX       (0 <<  5)
#define MIO_PIN_OPT_LVL31_MUX       (1 <<  5)
#define MIO_PIN_OPT_LVL32_MUX       (2 <<  5)
#define MIO_PIN_OPT_LVL33_MUX       (3 <<  5)
#define MIO_PIN_OPT_LVL34_MUX       (4 <<  5)
#define MIO_PIN_OPT_LVL35_MUX       (5 <<  5)
#define MIO_PIN_OPT_LVL36_MUX       (6 <<  5)
#define MIO_PIN_OPT_LVL37_MUX       (7 <<  5)
#define MIO_PIN_OPT_LVL23_MUX       (3 <<  3)
#define MIO_PIN_OPT_LVL22_MUX       (2 <<  3)
#define MIO_PIN_OPT_LVL21_MUX       (1 <<  3)
#define MIO_PIN_OPT_LVL11_MUX       (1 <<  2)
#define MIO_PIN_OPT_LVL01_MUX       (1 <<  1)
#define MIO_PIN_OPT_ENA_TRISATE     (1 <<  0)

#define SCLR_UNLOCK_KEY         (0xDF0D)
#define SCLR_LOCK_KEY           (0x767B)

#define SLCR_BASE_ADDR                    (0xF8000000)
#define REG_SLCR_LOCK                     *(volatile U32*)(SLCR_BASE_ADDR + 0x0004) // SLCR lock register
#define REG_SLCR_UNLOCK                   *(volatile U32*)(SLCR_BASE_ADDR + 0x0008) // SLCR unlock register
#define REG_SLCR_ARM_PLL_CTRL             *(volatile U32*)(SLCR_BASE_ADDR + 0x0100) // ARM PLL Control
#define REG_SLCR_DDR_PLL_CTRL             *(volatile U32*)(SLCR_BASE_ADDR + 0x0104) // DDR PLL Control
#define REG_SLCR_IO_PLL_CTRL              *(volatile U32*)(SLCR_BASE_ADDR + 0x0108) // IO PLL Control
#define REG_SLCR_PLL_STATUS               *(volatile U32*)(SLCR_BASE_ADDR + 0x010C) // PLL Status
#define REG_SLCR_ARM_PLL_CFG              *(volatile U32*)(SLCR_BASE_ADDR + 0x0110) // ARM PLL Configuration
#define REG_SLCR_DDR_PLL_CFG              *(volatile U32*)(SLCR_BASE_ADDR + 0x0114) // DDR PLL Configuration
#define REG_SLCR_IO_PLL_CFG               *(volatile U32*)(SLCR_BASE_ADDR + 0x0118) // IO PLL Configuration
#define REG_SLCR_ARM_CLK_CTRL             *(volatile U32*)(SLCR_BASE_ADDR + 0x0120) // CPU Clock Control
#define REG_SLCR_DDR_CLK_CTRL             *(volatile U32*)(SLCR_BASE_ADDR + 0x0124) // DDR Clock Control
#define REG_SLCR_DCI_CLK_CTRL             *(volatile U32*)(SLCR_BASE_ADDR + 0x0128)  // DCI clock control
#define REG_SLCR_USB0_CLK_CTRL            *(volatile U32*)(SLCR_BASE_ADDR + 0x0130) // USB 0 ULPI Clock Control
#define REG_SLCR_USB1_CLK_CTRL            *(volatile U32*)(SLCR_BASE_ADDR + 0x0134) // USB 1 ULPI Clock Control
#define REG_SLCR_APER_CLK_CTRL            *(volatile U32*)(SLCR_BASE_ADDR + 0x012C) // AMBA peripheral clock control
#define REG_SLCR_GEM0_RCLK_CTRL           *(volatile U32*)(SLCR_BASE_ADDR + 0x0138) // GigE 0 Rx Clock and Rx Signals Select
#define REG_SLCR_GEM1_CLK_CTRL            *(volatile U32*)(SLCR_BASE_ADDR + 0x013C) // GbE controller 1 clock control
#define REG_SLCR_GEM0_CLK_CTRL            *(volatile U32*)(SLCR_BASE_ADDR + 0x0140) // GigE 0 Ref Clock Control
#define REG_SLCR_SMC_CLK_CTRL             *(volatile U32*)(SLCR_BASE_ADDR + 0x0148) // SMC clock control
#define REG_SLCR_LQSPI_CLK_CTRL           *(volatile U32*)(SLCR_BASE_ADDR + 0x014C) // QSPI clock control
#define REG_SLCR_SDIO_CLK_CTRL            *(volatile U32*)(SLCR_BASE_ADDR + 0x0150) // SDIO clock control
#define REG_SLCR_UART_CLK_CTRL            *(volatile U32*)(SLCR_BASE_ADDR + 0x0154) // UART clock control
#define REG_SLCR_SPI_CLK_CTRL             *(volatile U32*)(SLCR_BASE_ADDR + 0x0158) // SPI1 clock control
#define REG_SLCR_CAN_CLK_CTRL             *(volatile U32*)(SLCR_BASE_ADDR + 0x015C) // CAN clock control
#define REG_SLCR_DBG_CLK_CTRL             *(volatile U32*)(SLCR_BASE_ADDR + 0x0164) // Debug clock control
#define REG_SLCR_PCAP_CLK_CTRL            *(volatile U32*)(SLCR_BASE_ADDR + 0x0168) // PCAP clock control
#define REG_SLCR_FPGA0_CLK_CTRL           *(volatile U32*)(SLCR_BASE_ADDR + 0x0170) // PL Clock 0 Output control
#define REG_SLCR_FPGA1_CLK_CTRL           *(volatile U32*)(SLCR_BASE_ADDR + 0x0180) // PL Clock 1 Output control
#define REG_SLCR_FPGA2_CLK_CTRL           *(volatile U32*)(SLCR_BASE_ADDR + 0x0190) // PL Clock 2 output control
#define REG_SLCR_FPGA3_CLK_CTRL           *(volatile U32*)(SLCR_BASE_ADDR + 0x01A0) // PL Clock 3 output control
#define REG_SLCR_CLK_621_TRUE             *(volatile U32*)(SLCR_BASE_ADDR + 0x01C4) // CPU Clock Ratio Mode select
#define REG_PSS_RST_CTRL                  *(volatile U32*)(SLCR_BASE_ADDR + 0x0200) // PS software reset control
#define REG_SLCR_DMAC_RST_CTRL            *(volatile U32*)(SLCR_BASE_ADDR + 0x020C) // DMAC Software Reset Control
#define REG_SLCR_USB_RST_CTRL             *(volatile U32*)(SLCR_BASE_ADDR + 0x0210) // USB Software Reset Control
#define REG_FPGA_RST_CTRL                 *(volatile U32*)(SLCR_BASE_ADDR + 0x0240) // FPGA software reset control
#define REG_SLCR_WDT_CLK_SEL              *(volatile U32*)(SLCR_BASE_ADDR + 0x0304) // System watchdog clock select
#define REG_SLCR_TZ_DMA_NS                *(volatile U32*)(SLCR_BASE_ADDR + 0x0440) // DMAC TrustZone Config
#define REG_SLCR_TZ_DMA_IRQ_NS            *(volatile U32*)(SLCR_BASE_ADDR + 0x0444) // DMAC TrustZone Config for Interrupts
#define REG_SLCR_TZ_DMA_PERIPH_NS         *(volatile U32*)(SLCR_BASE_ADDR + 0x0448) // DMAC TrustZone Config for Peripherals
#define REG_SLCR_DDR_URGENT               *(volatile U32*)(SLCR_BASE_ADDR + 0x0600) // DDR Urgent control
#define REG_SLCR_DDR_URGENT_SEL           *(volatile U32*)(SLCR_BASE_ADDR + 0x061C) // DDR Urgent select
#define REG_SLCR_MIO_PIN_00               *(volatile U32*)(SLCR_BASE_ADDR + 0x0700) // MIO Pin 0 Control
#define REG_SLCR_MIO_PIN_01               *(volatile U32*)(SLCR_BASE_ADDR + 0x0704) // MIO Pin 1 Control
#define REG_SLCR_MIO_PIN_02               *(volatile U32*)(SLCR_BASE_ADDR + 0x0708) // MIO Pin 2 Control
#define REG_SLCR_MIO_PIN_03               *(volatile U32*)(SLCR_BASE_ADDR + 0x070C) // MIO Pin 3 Control
#define REG_SLCR_MIO_PIN_04               *(volatile U32*)(SLCR_BASE_ADDR + 0x0710) // MIO Pin 4 Control
#define REG_SLCR_MIO_PIN_05               *(volatile U32*)(SLCR_BASE_ADDR + 0x0714) // MIO Pin 5 Control
#define REG_SLCR_MIO_PIN_06               *(volatile U32*)(SLCR_BASE_ADDR + 0x0718) // MIO Pin 6 Control
#define REG_SLCR_MIO_PIN_07               *(volatile U32*)(SLCR_BASE_ADDR + 0x071C) // MIO Pin 7 Control
#define REG_SLCR_MIO_PIN_08               *(volatile U32*)(SLCR_BASE_ADDR + 0x0720) // MIO Pin 8 Control
#define REG_SLCR_MIO_PIN_09               *(volatile U32*)(SLCR_BASE_ADDR + 0x0724) // MIO Pin 9 Control
#define REG_SLCR_MIO_PIN_10               *(volatile U32*)(SLCR_BASE_ADDR + 0x0728) // MIO Pin 10 Control
#define REG_SLCR_MIO_PIN_11               *(volatile U32*)(SLCR_BASE_ADDR + 0x072C) // MIO Pin 11 Control
#define REG_SLCR_MIO_PIN_12               *(volatile U32*)(SLCR_BASE_ADDR + 0x0730) // MIO Pin 12 Control
#define REG_SLCR_MIO_PIN_13               *(volatile U32*)(SLCR_BASE_ADDR + 0x0734) // MIO Pin 13 Control
#define REG_SLCR_MIO_PIN_14               *(volatile U32*)(SLCR_BASE_ADDR + 0x0738) // MIO Pin 14 Control
#define REG_SLCR_MIO_PIN_15               *(volatile U32*)(SLCR_BASE_ADDR + 0x073C) // MIO Pin 15 Control
#define REG_SLCR_MIO_PIN_16               *(volatile U32*)(SLCR_BASE_ADDR + 0x0740) // MIO Pin 16 Control
#define REG_SLCR_MIO_PIN_17               *(volatile U32*)(SLCR_BASE_ADDR + 0x0744) // MIO Pin 17 Control
#define REG_SLCR_MIO_PIN_18               *(volatile U32*)(SLCR_BASE_ADDR + 0x0748) // MIO Pin 18 Control
#define REG_SLCR_MIO_PIN_19               *(volatile U32*)(SLCR_BASE_ADDR + 0x074C) // MIO Pin 19 Control
#define REG_SLCR_MIO_PIN_20               *(volatile U32*)(SLCR_BASE_ADDR + 0x0750) // MIO Pin 20 Control
#define REG_SLCR_MIO_PIN_21               *(volatile U32*)(SLCR_BASE_ADDR + 0x0754) // MIO Pin 21 Control
#define REG_SLCR_MIO_PIN_22               *(volatile U32*)(SLCR_BASE_ADDR + 0x0758) // MIO Pin 22 Control
#define REG_SLCR_MIO_PIN_23               *(volatile U32*)(SLCR_BASE_ADDR + 0x075C) // MIO Pin 23 Control
#define REG_SLCR_MIO_PIN_24               *(volatile U32*)(SLCR_BASE_ADDR + 0x0760) // MIO Pin 24 Control
#define REG_SLCR_MIO_PIN_25               *(volatile U32*)(SLCR_BASE_ADDR + 0x0764) // MIO Pin 25 Control
#define REG_SLCR_MIO_PIN_26               *(volatile U32*)(SLCR_BASE_ADDR + 0x0768) // MIO Pin 26 Control
#define REG_SLCR_MIO_PIN_27               *(volatile U32*)(SLCR_BASE_ADDR + 0x076C) // MIO Pin 27 Control
#define REG_SLCR_MIO_PIN_28               *(volatile U32*)(SLCR_BASE_ADDR + 0x0770) // MIO Pin 28 Control
#define REG_SLCR_MIO_PIN_29               *(volatile U32*)(SLCR_BASE_ADDR + 0x0774) // MIO Pin 29 Control
#define REG_SLCR_MIO_PIN_30               *(volatile U32*)(SLCR_BASE_ADDR + 0x0778) // MIO Pin 30 Control
#define REG_SLCR_MIO_PIN_31               *(volatile U32*)(SLCR_BASE_ADDR + 0x077C) // MIO Pin 31 Control
#define REG_SLCR_MIO_PIN_32               *(volatile U32*)(SLCR_BASE_ADDR + 0x0780) // MIO Pin 32 Control
#define REG_SLCR_MIO_PIN_33               *(volatile U32*)(SLCR_BASE_ADDR + 0x0784) // MIO Pin 33 Control
#define REG_SLCR_MIO_PIN_34               *(volatile U32*)(SLCR_BASE_ADDR + 0x0788) // MIO Pin 34 Control
#define REG_SLCR_MIO_PIN_35               *(volatile U32*)(SLCR_BASE_ADDR + 0x078C) // MIO Pin 35 Control
#define REG_SLCR_MIO_PIN_36               *(volatile U32*)(SLCR_BASE_ADDR + 0x0790) // MIO Pin 36 Control
#define REG_SLCR_MIO_PIN_37               *(volatile U32*)(SLCR_BASE_ADDR + 0x0794) // MIO Pin 37 Control
#define REG_SLCR_MIO_PIN_38               *(volatile U32*)(SLCR_BASE_ADDR + 0x0798) // MIO Pin 38 Control
#define REG_SLCR_MIO_PIN_39               *(volatile U32*)(SLCR_BASE_ADDR + 0x079C) // MIO Pin 39 Control
#define REG_SLCR_MIO_PIN_40               *(volatile U32*)(SLCR_BASE_ADDR + 0x07A0) // MIO Pin 40 Control
#define REG_SLCR_MIO_PIN_41               *(volatile U32*)(SLCR_BASE_ADDR + 0x07A4) // MIO Pin 41 Control
#define REG_SLCR_MIO_PIN_42               *(volatile U32*)(SLCR_BASE_ADDR + 0x07A8) // MIO Pin 42 Control
#define REG_SLCR_MIO_PIN_43               *(volatile U32*)(SLCR_BASE_ADDR + 0x07AC) // MIO Pin 43 Control
#define REG_SLCR_MIO_PIN_44               *(volatile U32*)(SLCR_BASE_ADDR + 0x07B0) // MIO Pin 44 Control
#define REG_SLCR_MIO_PIN_45               *(volatile U32*)(SLCR_BASE_ADDR + 0x07B4) // MIO Pin 45 Control
#define REG_SLCR_MIO_PIN_46               *(volatile U32*)(SLCR_BASE_ADDR + 0x07B8) // MIO Pin 46 Control
#define REG_SLCR_MIO_PIN_47               *(volatile U32*)(SLCR_BASE_ADDR + 0x07BC) // MIO Pin 47 Control
#define REG_SLCR_MIO_PIN_48               *(volatile U32*)(SLCR_BASE_ADDR + 0x07C0) // MIO Pin 48 Control
#define REG_SLCR_MIO_PIN_49               *(volatile U32*)(SLCR_BASE_ADDR + 0x07C4) // MIO Pin 49 Control
#define REG_SLCR_MIO_PIN_50               *(volatile U32*)(SLCR_BASE_ADDR + 0x07C8) // MIO Pin 50 Control
#define REG_SLCR_MIO_PIN_51               *(volatile U32*)(SLCR_BASE_ADDR + 0x07CC) // MIO Pin 51 Control
#define REG_SLCR_MIO_PIN_52               *(volatile U32*)(SLCR_BASE_ADDR + 0x07D0) // MIO Pin 52 Control
#define REG_SLCR_MIO_PIN_53               *(volatile U32*)(SLCR_BASE_ADDR + 0x07D4) // MIO Pin 53 Control
#define REG_SLCR_LVL_SHFTR_EN             *(volatile U32*)(SLCR_BASE_ADDR + 0x0900) // Enable for level shifters between PS and PL
#define REG_SLCR_OCM_CFG                  *(volatile U32*)(SLCR_BASE_ADDR + 0x0910) // OCM Address Mapping
#define REG_SLCR_RESERVED                 *(volatile U32*)(SLCR_BASE_ADDR + 0x0A1C) // (Reserved); required for L2 Cache config. (see TRM, section 3.4.10 "Programming Model")
#define REG_SLCR_DDRIOB_ADDR0             *(volatile U32*)(SLCR_BASE_ADDR + 0x0B40)
#define REG_SLCR_DDRIOB_ADDR1             *(volatile U32*)(SLCR_BASE_ADDR + 0x0B44)  // DDR IOB Config for BA[2:0], ODT, CS_B, WE_B, RAS_B and CAS_B
#define REG_SLCR_DDRIOB_DATA0             *(volatile U32*)(SLCR_BASE_ADDR + 0x0B48)  // DDR IOB Config for Data 15:0
#define REG_SLCR_DDRIOB_DATA1             *(volatile U32*)(SLCR_BASE_ADDR + 0x0B4C)  // DDR IOB Config for Data 31:16
#define REG_SLCR_DDRIOB_DIFF0             *(volatile U32*)(SLCR_BASE_ADDR + 0x0B50)  // DDR IOB Config for DQS 1:0
#define REG_SLCR_DDRIOB_DIFF1             *(volatile U32*)(SLCR_BASE_ADDR + 0x0B54)  // DDR IOB Config for DQS 3:2
#define REG_SLCR_DDRIOB_CLOCK             *(volatile U32*)(SLCR_BASE_ADDR + 0x0B58)  // DDR IOB Config for Clock Output
#define REG_SLCR_DDRIOB_DRIVE_SLEW_ADDR   *(volatile U32*)(SLCR_BASE_ADDR + 0x0B5C)  // Drive and Slew controls for Address and Command pins of the DDR Interface
#define REG_SLCR_DDRIOB_DRIVE_SLEW_DATA   *(volatile U32*)(SLCR_BASE_ADDR + 0x0B60)  // Drive and Slew controls for DQ  pins of the DDR Interface
#define REG_SLCR_DDRIOB_DRIVE_SLEW_DIFF   *(volatile U32*)(SLCR_BASE_ADDR + 0x0B64)  // Drive and Slew controls for DQS  pins of the DDR Interface
#define REG_SLCR_DDRIOB_DRIVE_SLEW_CLOCK  *(volatile U32*)(SLCR_BASE_ADDR + 0x0B68)  // Drive and Slew controls for Clock pins of the DDR Interface
#define REG_SLCR_DDRIOB_DDR_CTRL          *(volatile U32*)(SLCR_BASE_ADDR + 0x0B6C)  // DDR IOB Buffer Control
#define REG_SLCR_DDRIOB_DCI_CTRL          *(volatile U32*)(SLCR_BASE_ADDR + 0x0B70)  // DDR IOB DCI Config
#define REG_SLCR_DDRIOB_DCI_STATUS        *(volatile U32*)(SLCR_BASE_ADDR + 0x0B74)  // DDR IO Buffer DCI Status

/****** AXI High Priority Port 0 ****************************************/
#define AFI_HP0_BASE_ADDR       0xF8008000
#define REG_HP0_WRCHAN_CTRL     *(volatile U32*)(AFI_HP0_BASE_ADDR + 0x0014) // AXI HP0 write channel control
#define REG_HP0_WRCHAN_ISSCAP   *(volatile U32*)(AFI_HP0_BASE_ADDR + 0x0018) // AXI HP0 write channel issuing capability
#define REG_HP0_WRQOS           *(volatile U32*)(AFI_HP0_BASE_ADDR + 0x001C) // AXI HP0 write channel Quality of Service
#define REG_HP0_WRDATAFIFO_LVL  *(volatile U32*)(AFI_HP0_BASE_ADDR + 0x0020) // AXI HP0 write data FIFO level

/****** Device Configuration Port ***************************************/
#define DEVC_BASE_ADDR          0xF8007000
#define REG_DEVC_CTRL           *(volatile U32*)(DEVC_BASE_ADDR + 0x0000) // Device configuration control
#define REG_DEVC_CFG            *(volatile U32*)(DEVC_BASE_ADDR + 0x0008) // Device configuration configuration
#define REG_DEVC_INT_STS        *(volatile U32*)(DEVC_BASE_ADDR + 0x000C) // Device configuration interrupt status
#define REG_DEVC_STATUS         *(volatile U32*)(DEVC_BASE_ADDR + 0x0014) // Device configuration status
#define REG_DEVC_DMA_SRC_ADDR   *(volatile U32*)(DEVC_BASE_ADDR + 0x0018) // Device configuration DMA source address
#define REG_DEVC_DMA_DST_ADDR   *(volatile U32*)(DEVC_BASE_ADDR + 0x001C) // Device configuration DMA destination address
#define REG_DEVC_DMA_SRC_LEN    *(volatile U32*)(DEVC_BASE_ADDR + 0x0020) // Device configuration DMA source length (words)
#define REG_DEVC_DMA_DST_LEN    *(volatile U32*)(DEVC_BASE_ADDR + 0x0024) // Device configuration DMA destination length (words)
#define REG_DEVC_MCTRL          *(volatile U32*)(DEVC_BASE_ADDR + 0x0080) // Device configuration miscellaneous control
#define REG_XADCIF_CFG          *(volatile U32*)(DEVC_BASE_ADDR + 0x0100) // XADC interface configuration
#define REG_XADCIF_INT_STS      *(volatile U32*)(DEVC_BASE_ADDR + 0x0104) // XADC interface interrupt status
#define REG_XADCIF_INT_MASK     *(volatile U32*)(DEVC_BASE_ADDR + 0x0108) // XADC interface interrupt mask
#define REG_XADCIF_INT_MSTS     *(volatile U32*)(DEVC_BASE_ADDR + 0x010C) // XADC interface miscellaneous status
#define REG_XADCIF_CMDFIFO      *(volatile U32*)(DEVC_BASE_ADDR + 0x0110) // XADC interface command FIFO
#define REG_XADCIF_RDFIFO       *(volatile U32*)(DEVC_BASE_ADDR + 0x0114) // XADC interface data FIFO
#define REG_XADCIF_MCTL         *(volatile U32*)(DEVC_BASE_ADDR + 0x0118) // XADC interface miscellaneous control

/****** Triple Timer Counter 0 ******************************************/
#define TTC0_BASE_ADDR          0xF8001000
#define REG_TTC0_CLOCK_CTRL1    *(volatile U32*)(TTC0_BASE_ADDR + 0x0000)
#define REG_TTC0_CLOCK_CTRL2    *(volatile U32*)(TTC0_BASE_ADDR + 0x0004)
#define REG_TTC0_CLOCK_CTRL3    *(volatile U32*)(TTC0_BASE_ADDR + 0x0008)
#define REG_TTC0_COUNTER_CTRL1  *(volatile U32*)(TTC0_BASE_ADDR + 0x000C)
#define REG_TTC0_COUNTER_CTRL2  *(volatile U32*)(TTC0_BASE_ADDR + 0x0010)
#define REG_TTC0_COUNTER_CTRL3  *(volatile U32*)(TTC0_BASE_ADDR + 0x0014)
#define REG_TTC0_COUNTER_VAL1   *(volatile U32*)(TTC0_BASE_ADDR + 0x0018)
#define REG_TTC0_COUNTER_VAL2   *(volatile U32*)(TTC0_BASE_ADDR + 0x001C)
#define REG_TTC0_COUNTER_VAL3   *(volatile U32*)(TTC0_BASE_ADDR + 0x0020)
#define REG_TTC0_INTERV_CNTR1   *(volatile U32*)(TTC0_BASE_ADDR + 0x0024)
#define REG_TTC0_INTERV_CNTR2   *(volatile U32*)(TTC0_BASE_ADDR + 0x0028)
#define REG_TTC0_INTERV_CNTR3   *(volatile U32*)(TTC0_BASE_ADDR + 0x002C)
#define REG_TTC0_MATCH1_CNTR1   *(volatile U32*)(TTC0_BASE_ADDR + 0x0030)
#define REG_TTC0_MATCH1_CNTR2   *(volatile U32*)(TTC0_BASE_ADDR + 0x0034)
#define REG_TTC0_MATCH1_CNTR3   *(volatile U32*)(TTC0_BASE_ADDR + 0x0038)
#define REG_TTC0_ISR1           *(volatile U32*)(TTC0_BASE_ADDR + 0x0054)
#define REG_TTC0_ISR2           *(volatile U32*)(TTC0_BASE_ADDR + 0x0058)
#define REG_TTC0_ISR3           *(volatile U32*)(TTC0_BASE_ADDR + 0x005C)
#define REG_TTC0_IER1           *(volatile U32*)(TTC0_BASE_ADDR + 0x0060)
#define REG_TTC0_IER2           *(volatile U32*)(TTC0_BASE_ADDR + 0x0064)
#define REG_TTC0_IER3           *(volatile U32*)(TTC0_BASE_ADDR + 0x0068)

/****** System Watchdog Timer *******************************************/
#define SWDT_BASE_ADDR          0xF8005000

#define SWDT_RESTART_VALUE      (0x1999)

#define REG_SWDT_MODE           *(volatile U32*)(SWDT_BASE_ADDR + 0x0000)
#define REG_SWDT_CONTROL        *(volatile U32*)(SWDT_BASE_ADDR + 0x0004)
#define REG_SWDT_RESTART        *(volatile U32*)(SWDT_BASE_ADDR + 0x0008)
#define REG_SWDT_STATUS         *(volatile U32*)(SWDT_BASE_ADDR + 0x000C)

/****** XADC (Using AXI4-Lite Interface) ********************************/
#define XADC_BASE_ADDR          0x43C10000    // Defined in Vivado
#define REG_XADC_WR_SRR         *(volatile U32*)(XADC_BASE_ADDR + 0x0000) // XADC software reset
#define REG_XADC_RD_SR          *(volatile U32*)(XADC_BASE_ADDR + 0x0004) // XADC status register
#define REG_XADC_WR_CONVSTR     *(volatile U32*)(XADC_BASE_ADDR + 0x000C) // XADC conversion start
#define REG_XADC_WR_SYSMONRR    *(volatile U32*)(XADC_BASE_ADDR + 0x0010) // XADC hard macro reset
#define REG_XADC_RW_GIER        *(volatile U32*)(XADC_BASE_ADDR + 0x005C) // XADC global interrupt enable
#define REG_XADC_RW_IPISR       *(volatile U32*)(XADC_BASE_ADDR + 0x0060) // XADC interrupt status
#define REG_XADC_RW_IPIER       *(volatile U32*)(XADC_BASE_ADDR + 0x0068) // XADC interrupt enable
#define REG_XADC_RD_TEMP        *(volatile U32*)(XADC_BASE_ADDR + 0x0200) // XADC status: Temperature
#define REG_XADC_RD_VCCINT      *(volatile U32*)(XADC_BASE_ADDR + 0x0204) // XADC status: VCCINT
#define REG_XADC_RD_VCCAUX      *(volatile U32*)(XADC_BASE_ADDR + 0x0208) // XADC status: VCCAUX
#define REG_XADC_RD_VCCPINT     *(volatile U32*)(XADC_BASE_ADDR + 0x0234) // XADC status: VCCPINT
#define REG_XADC_RD_VCCPAUX     *(volatile U32*)(XADC_BASE_ADDR + 0x0238) // XADC status: VCCPAUX
#define REG_XADC_RD_VCCDDR      *(volatile U32*)(XADC_BASE_ADDR + 0x023C) // XADC status: VCCDDR
#define REG_XADC_RD_VDIFF0      *(volatile U32*)(XADC_BASE_ADDR + 0x0248) // XADC status: VDIFF0
#define REG_XADC_RD_VDIFF1      *(volatile U32*)(XADC_BASE_ADDR + 0x024C) // XADC status: VDIFF1
#define REG_XADC_RD_VDIFF2      *(volatile U32*)(XADC_BASE_ADDR + 0x026C) // XADC status: VDIFF2
#define REG_XADC_RD_VTREF       *(volatile U32*)(XADC_BASE_ADDR + 0x0270) // XADC status: VTREF
#define REG_XADC_RD_VTSUPPLY    *(volatile U32*)(XADC_BASE_ADDR + 0x0254) // XADC status: VTSUPPLY
#define REG_XADC_RD_VBUS        *(volatile U32*)(XADC_BASE_ADDR + 0x0274) // XADC status: VBUS
#define REG_XADC_RD_TEMP_MAX    *(volatile U32*)(XADC_BASE_ADDR + 0x0280) // XADC status: Max. temperature
#define REG_XADC_RD_VCCINT_MAX  *(volatile U32*)(XADC_BASE_ADDR + 0x0284) // XADC status: Max. VCCINT
#define REG_XADC_RD_VCCAUX_MAX  *(volatile U32*)(XADC_BASE_ADDR + 0x0288) // XADC status: Max. VCCAUX
#define REG_XADC_RD_TEMP_MIN    *(volatile U32*)(XADC_BASE_ADDR + 0x0290) // XADC status: Min. temperature
#define REG_XADC_RD_VCCINT_MIN  *(volatile U32*)(XADC_BASE_ADDR + 0x0294) // XADC status: Min. VCCINT
#define REG_XADC_RD_VCCAUX_MIN  *(volatile U32*)(XADC_BASE_ADDR + 0x0298) // XADC status: Min. VCCAUX
#define REG_XADC_RD_VCCPINT_MAX *(volatile U32*)(XADC_BASE_ADDR + 0x02A0) // XADC status: Max. VCCPINT
#define REG_XADC_RD_VCCPAUX_MAX *(volatile U32*)(XADC_BASE_ADDR + 0x02A4) // XADC status: Max. VCCPAUX
#define REG_XADC_RD_VCCDDR_MAX  *(volatile U32*)(XADC_BASE_ADDR + 0x02A8) // XADC status: Max. VCCDDR
#define REG_XADC_RD_VCCPINT_MIN *(volatile U32*)(XADC_BASE_ADDR + 0x02B0) // XADC status: Min. VCCPINT
#define REG_XADC_RD_VCCPAUX_MIN *(volatile U32*)(XADC_BASE_ADDR + 0x02B4) // XADC status: Min. VCCPAUX
#define REG_XADC_RD_VCCDDR_MIN  *(volatile U32*)(XADC_BASE_ADDR + 0x02B8) // XADC status: Min. VCCDDR
#define REG_XADC_WR_CFG0        *(volatile U32*)(XADC_BASE_ADDR + 0x0300) // XADC configuration register 0
#define REG_XADC_WR_CFG1        *(volatile U32*)(XADC_BASE_ADDR + 0x0304) // XADC configuration register 1
#define REG_XADC_WR_CFG2        *(volatile U32*)(XADC_BASE_ADDR + 0x0308) // XADC configuration register 2
#define REG_XADC_WR_SEQ0        *(volatile U32*)(XADC_BASE_ADDR + 0x0320) // XADC sequence register 0
#define REG_XADC_WR_SEQ1        *(volatile U32*)(XADC_BASE_ADDR + 0x0324) // XADC sequence register 1
#define REG_XADC_WR_SEQ2        *(volatile U32*)(XADC_BASE_ADDR + 0x0328) // XADC sequence register 2
#define REG_XADC_WR_SEQ3        *(volatile U32*)(XADC_BASE_ADDR + 0x032C) // XADC sequence register 3
#define REG_XADC_WR_SEQ4        *(volatile U32*)(XADC_BASE_ADDR + 0x0330) // XADC sequence register 4
#define REG_XADC_WR_SEQ5        *(volatile U32*)(XADC_BASE_ADDR + 0x0334) // XADC sequence register 5
#define REG_XADC_WR_SEQ6        *(volatile U32*)(XADC_BASE_ADDR + 0x0338) // XADC sequence register 6
#define REG_XADC_WR_SEQ7        *(volatile U32*)(XADC_BASE_ADDR + 0x033C) // XADC sequence register 7

/****** QSPI controller **********************************************************/
#define QSPIC_BASE_ADDR (0xE000D000)
#define REG_QSPIC_CONFIG_REG              *(volatile U32*)(QSPIC_BASE_ADDR + 0x0000) // QSPI configuration register
#define REG_QSPIC_INTR_STATUS_REG         *(volatile U32*)(QSPIC_BASE_ADDR + 0x0004) // QSPI interrupt status register
#define REG_QSPIC_INTRPT_EN_REG           *(volatile U32*)(QSPIC_BASE_ADDR + 0x0008) // Interrupt Enable register.
#define REG_QSPIC_INTRPT_DIS_REG          *(volatile U32*)(QSPIC_BASE_ADDR + 0x000C) // Interrupt disable register.
#define REG_QSPIC_INTRPT_MASK_REG         *(volatile U32*)(QSPIC_BASE_ADDR + 0x0010) // Interrupt mask register
#define REG_QSPIC_EN_REG                  *(volatile U32*)(QSPIC_BASE_ADDR + 0x0014) // SPI_Enable Register
#define REG_QSPIC_DELAY_REG               *(volatile U32*)(QSPIC_BASE_ADDR + 0x0018) // Delay Register
#define REG_QSPIC_TXD0                    *(volatile U32*)(QSPIC_BASE_ADDR + 0x001C) // Transmit Data Register. Keyhole addresses for the Transmit data FIFO. See also TXD1-3.
#define REG_QSPIC_RX_DATA_REG             *(volatile U32*)(QSPIC_BASE_ADDR + 0x0020) // Receive Data Register
#define REG_QSPIC_SLAVE_IDLE_COUNT_REG    *(volatile U32*)(QSPIC_BASE_ADDR + 0x0024) // Slave Idle Count Register
#define REG_QSPIC_TX_THRES_REG            *(volatile U32*)(QSPIC_BASE_ADDR + 0x0028) // TX_FIFO Threshold Register
#define REG_QSPIC_RX_THRES_REG            *(volatile U32*)(QSPIC_BASE_ADDR + 0x002C) // RX FIFO Threshold Register
#define REG_QSPIC_GPIO                    *(volatile U32*)(QSPIC_BASE_ADDR + 0x0030) // General Purpose Inputs and Outputs Register for the Quad-SPI Controller core
#define REG_QSPIC_LPBK_DLY_ADJ            *(volatile U32*)(QSPIC_BASE_ADDR + 0x0038) // Loopback Master Clock Delay Adjustment Register
#define REG_QSPIC_TXD1                    *(volatile U32*)(QSPIC_BASE_ADDR + 0x0080) // Transmit Data Register. Keyhole addresses for the Transmit data FIFO.
#define REG_QSPIC_TXD2                    *(volatile U32*)(QSPIC_BASE_ADDR + 0x0084) // Transmit Data Register. Keyhole addresses for the Transmit data FIFO.
#define REG_QSPIC_TXD3                    *(volatile U32*)(QSPIC_BASE_ADDR + 0x0088) // Transmit Data Register. Keyhole addresses for the Transmit data FIFO.
#define REG_QSPIC_LQSPI_CFG               *(volatile U32*)(QSPIC_BASE_ADDR + 0x00A0) // Configuration Register specifically for the Linear Quad-SPI Controller
#define REG_QSPIC_LQSPI_STS               *(volatile U32*)(QSPIC_BASE_ADDR + 0x00A4) // Status Register specifically for the Linear Quad-SPI Controller
#define REG_QSPIC_MOD_ID                  *(volatile U32*)(QSPIC_BASE_ADDR + 0x00FC) // Module Identification register

/****** MPCORE **********************************************************/
#define MPCORE_BASE_ADDR                     (0xF8F00000)
#define MPCORE_SCU_CONTROL                   *((volatile U32*)(MPCORE_BASE_ADDR + 0x0000))
#define MPCORE_ICDICFR2                      *((volatile U32*)(MPCORE_BASE_ADDR + 0x1C08)) // Interrupt sensitivity control IRQ ID32-47
#define MPCORE_SCU_CONTROL                   *((volatile U32*)(MPCORE_BASE_ADDR + 0x0000))
#define MPCORE_WATCHDOG_LOAD_REG             *((volatile U32*)(MPCORE_BASE_ADDR + 0x0620))
#define MPCORE_WATCHDOG_COUNTER_REG          *((volatile U32*)(MPCORE_BASE_ADDR + 0x0624))
#define MPCORE_WATCHDOG_CONTROL_REG          *((volatile U32*)(MPCORE_BASE_ADDR + 0x0628))
#define MPCORE_WATCHDOG_INTERRUPT_STATUS_REG *((volatile U32*)(MPCORE_BASE_ADDR + 0x062C))
#define MPCORE_WATCHDOG_RESET_STATUS_REG     *((volatile U32*)(MPCORE_BASE_ADDR + 0x0630))
#define MPCORE_WATCHDOG_DISABLE_REG          *((volatile U32*)(MPCORE_BASE_ADDR + 0x0634))

/****** L2 Cache Controller *********************************************/
#define xxL2CACHE_BASE_ADDR       0xF8F02000
#define L2C_REG1_CONTROL        *(volatile U32*)(L2CACHE_BASE_ADDR + 0x0100)
#define L2C_REG1_AUX_CONTROL    *(volatile U32*)(L2CACHE_BASE_ADDR + 0x0104)
#define L2C_REG1_TAG_RAM_CTRL   *(volatile U32*)(L2CACHE_BASE_ADDR + 0x0108)
#define L2C_REG1_DATA_RAM_CTRL  *(volatile U32*)(L2CACHE_BASE_ADDR + 0x010C)
#define L2C_REG2_INT_RAW_STATUS *(volatile U32*)(L2CACHE_BASE_ADDR + 0x021C)
#define L2C_REG2_INT_CLEAR      *(volatile U32*)(L2CACHE_BASE_ADDR + 0x0220)
#define L2C_DUMMY_CACHE_SYNC    *(volatile U32*)(L2CACHE_BASE_ADDR + 0x0740)
#define L2C_REG7_INV_PA         *(volatile U32*)(L2CACHE_BASE_ADDR + 0x0770)
#define L2C_REG7_INV_WAY        *(volatile U32*)(L2CACHE_BASE_ADDR + 0x077C)
#define L2C_REG7_CLEAN_PA       *(volatile U32*)(L2CACHE_BASE_ADDR + 0x07B0)
#define L2C_REG7_CLEAN_WAY      *(volatile U32*)(L2CACHE_BASE_ADDR + 0x07BC)
#define L2C_REG7_CLEAN_INV_PA   *(volatile U32*)(L2CACHE_BASE_ADDR + 0x07F0)
#define L2C_REG7_CLEAN_INV_WAY  *(volatile U32*)(L2CACHE_BASE_ADDR + 0x07FC)
#define L2C_REG15_DBG_CTRL      *(volatile U32*)(L2CACHE_BASE_ADDR + 0x0F40)
#define L2C_REG15_POWER_CTRL    *(volatile U32*)(L2CACHE_BASE_ADDR + 0x0F80)

/****** DEVCFG registers *****************************************/
#define DEVCFG_BASE_ADDR (0xF8007000)
#define REG_DEVCFG_CTRL             *(volatile U32*)(DEVCFG_BASE_ADDR + 0x00000000)
#define REG_DEVCFG_LOCK             *(volatile U32*)(DEVCFG_BASE_ADDR + 0x00000004)
#define REG_DEVCFG_CFG              *(volatile U32*)(DEVCFG_BASE_ADDR + 0x00000008)
#define REG_DEVCFG_INT_STS          *(volatile U32*)(DEVCFG_BASE_ADDR + 0x0000000C)
#define REG_DEVCFG_INT_MASK         *(volatile U32*)(DEVCFG_BASE_ADDR + 0x00000010)
#define REG_DEVCFG_STATUS           *(volatile U32*)(DEVCFG_BASE_ADDR + 0x00000014)
#define REG_DEVCFG_DMA_SRC_ADDR     *(volatile U32*)(DEVCFG_BASE_ADDR + 0x00000018)
#define REG_DEVCFG_DMA_DST_ADDR     *(volatile U32*)(DEVCFG_BASE_ADDR + 0x0000001C)
#define REG_DEVCFG_DMA_SRC_LEN      *(volatile U32*)(DEVCFG_BASE_ADDR + 0x00000020)
#define REG_DEVCFG_DMA_DEST_LEN     *(volatile U32*)(DEVCFG_BASE_ADDR + 0x00000024)
#define REG_DEVCFG_MULTIBOOT_ADDR   *(volatile U32*)(DEVCFG_BASE_ADDR + 0x0000002C)
#define REG_DEVCFG_MULTIBOOT_ADDR_BYTE0 *(volatile unsigned char*) (0xF8007000 + 0x2C)
#define REG_DEVCFG_MULTIBOOT_ADDR_BYTE1 *(volatile unsigned char*) (0xF8007000 + 0x2D)
#define REG_DEVCFG_MULTIBOOT_ADDR_BYTE2 *(volatile unsigned char*) (0xF8007000 + 0x2E)
#define REG_DEVCFG_MULTIBOOT_ADDR_BYTE3 *(volatile unsigned char*) (0xF8007000 + 0x2F)
#define REG_DEVCFG_UNLOCK           *(volatile U32*)(DEVCFG_BASE_ADDR + 0x00000034)
#define REG_DEVCFG_MCTRL            *(volatile U32*)(DEVCFG_BASE_ADDR + 0x00000080)
#define REG_DEVCFG_XADCIF_CFG       *(volatile U32*)(DEVCFG_BASE_ADDR + 0x00000100)
#define REG_DEVCFG_XADCIF_INT_STS   *(volatile U32*)(DEVCFG_BASE_ADDR + 0x00000104)
#define REG_DEVCFG_XADCIF_INT_MASK  *(volatile U32*)(DEVCFG_BASE_ADDR + 0x00000108)
#define REG_DEVCFG_XADCIF_MSTS      *(volatile U32*)(DEVCFG_BASE_ADDR + 0x0000010C)
#define REG_DEVCFG_XADCIF_CMDFIFO   *(volatile U32*)(DEVCFG_BASE_ADDR + 0x00000110)
#define REG_DEVCFG_XADCIF_RDFIFO    *(volatile U32*)(DEVCFG_BASE_ADDR + 0x00000114)
#define REG_DEVCFG_XADCIF_MCTL      *(volatile U32*)(DEVCFG_BASE_ADDR + 0x00000118)

/****** USB registers *****************************************/

#define USB0_BASE_ADDR                           (0xE0002000)
#define REG_USB0_ID                              *(volatile U32*)(USB0_BASE_ADDR + 0x00000000) // IP version and revision, read-only
#define REG_USB0_HWGENERAL                       *(volatile U32*)(USB0_BASE_ADDR + 0x00000004) // Misc IP config constants, read-only
#define REG_USB0_HWHOST                          *(volatile U32*)(USB0_BASE_ADDR + 0x00000008) // Host Mode IP config constants, read-only
#define REG_USB0_HWDEVICE                        *(volatile U32*)(USB0_BASE_ADDR + 0x0000000C) // Device Mode IP config constants, read-only
#define REG_USB0_HWTXBUF                         *(volatile U32*)(USB0_BASE_ADDR + 0x00000010) // TxBuffer IP config constants, read-only
#define REG_USB0_HWRXBUF                         *(volatile U32*)(USB0_BASE_ADDR + 0x00000014) // IP constants, RX buffer constants, read-only
#define REG_USB0_GPTIMER0LD                      *(volatile U32*)(USB0_BASE_ADDR + 0x00000080) // GP Timer 0 Load Value
#define REG_USB0_GPTIMER0CTRL                    *(volatile U32*)(USB0_BASE_ADDR + 0x00000084) // GP Timer 1 Control
#define REG_USB0_GPTIMER1LD                      *(volatile U32*)(USB0_BASE_ADDR + 0x00000088) // GP Timer 1 Load Value
#define REG_USB0_GPTIMER1CTRL                    *(volatile U32*)(USB0_BASE_ADDR + 0x0000008C) // GP Timer 1 Control
#define REG_USB0_SBUSCFG                         *(volatile U32*)(USB0_BASE_ADDR + 0x00000090) // DMA Master AHB Burst Mode
#define REG_USB0_CAPLENGTH_HCIVERSION            *(volatile U32*)(USB0_BASE_ADDR + 0x00000100) // EHCI Addr Space and HCI constants, read-only
#define REG_USB0_HCSPARAMS                       *(volatile U32*)(USB0_BASE_ADDR + 0x00000104) // TT counts and EHCI HCS constants, read-only
#define REG_USB0_HCCPARAMS                       *(volatile U32*)(USB0_BASE_ADDR + 0x00000108) // EHCI Host configuration constants
#define REG_USB0_DCIVERSION                      *(volatile U32*)(USB0_BASE_ADDR + 0x00000120) // Device Mode CI version constant
#define REG_USB0_DCCPARAMS                       *(volatile U32*)(USB0_BASE_ADDR + 0x00000124) // EHCI, device and endpoint capabilities
#define REG_USB0_USBCMD                          *(volatile U32*)(USB0_BASE_ADDR + 0x00000140) // USB Commands (EHCI extended)
#define REG_USB0_USBSTS                          *(volatile U32*)(USB0_BASE_ADDR + 0x00000144) // Interrupt/Raw Status (EHCI extended) (Host/Device)
#define REG_USB0_USBINTR                         *(volatile U32*)(USB0_BASE_ADDR + 0x00000148) // Interrrupts and Enables
#define REG_USB0_FRINDEX                         *(volatile U32*)(USB0_BASE_ADDR + 0x0000014C) // Frame List Index
#define REG_USB0_PERIODICLISTBASE_DEVICEADDR     *(volatile U32*)(USB0_BASE_ADDR + 0x00000154) // Host/Device Address dual-use
#define REG_USB0_ASYNCLISTADDR_ENDPOINTLISTADDR  *(volatile U32*)(USB0_BASE_ADDR + 0x00000158) // Host/Device dual-use
#define REG_USB0_TTCTRL                          *(volatile U32*)(USB0_BASE_ADDR + 0x0000015C) // TT Control
#define REG_USB0_BURSTSIZE                       *(volatile U32*)(USB0_BASE_ADDR + 0x00000160) // Burst Size
#define REG_USB0_TXFILLTUNING                    *(volatile U32*)(USB0_BASE_ADDR + 0x00000164) // TxFIFO Fill Tuning
#define REG_USB0_TXTTFILLTUNING                  *(volatile U32*)(USB0_BASE_ADDR + 0x00000168) // TT TX latency FIFO
#define REG_USB0_IC_USB                          *(volatile U32*)(USB0_BASE_ADDR + 0x0000016C) // Low and Fast Speed Control constants
#define REG_USB0_ULPI_VIEWPORT                   *(volatile U32*)(USB0_BASE_ADDR + 0x00000170) // ULPI Viewport
#define REG_USB0_ENDPTNAK                        *(volatile U32*)(USB0_BASE_ADDR + 0x00000178) // Endpoint NAK (Device mode)
#define REG_USB0_ENDPTNAKEN                      *(volatile U32*)(USB0_BASE_ADDR + 0x0000017C) // Endpoint NAK (Device mode)
#define REG_USB0_CONFIGFLAG                      *(volatile U32*)(USB0_BASE_ADDR + 0x00000180) // reserved
#define REG_USB0_PORTSC1                         *(volatile U32*)(USB0_BASE_ADDR + 0x00000184) // Port Status & Control
#define REG_USB0_OTGSC                           *(volatile U32*)(USB0_BASE_ADDR + 0x000001A4) // OTG Status and Control
#define REG_USB0_USBMODE                         *(volatile U32*)(USB0_BASE_ADDR + 0x000001A8) // USB Mode Selection
#define REG_USB0_ENDPTSETUPSTAT                  *(volatile U32*)(USB0_BASE_ADDR + 0x000001AC) // Endpoint Status Setup (Device mode)
#define REG_USB0_ENDPTPRIME                      *(volatile U32*)(USB0_BASE_ADDR + 0x000001B0) // Endpoint Primer (Device mode)
#define REG_USB0_ENDPTFLUSH                      *(volatile U32*)(USB0_BASE_ADDR + 0x000001B4) // Endpoint Flush (Device mode)
#define REG_USB0_ENDPTSTAT                       *(volatile U32*)(USB0_BASE_ADDR + 0x000001B8) // Endpoint Buffer Ready Status (Device mode), RO
#define REG_USB0_ENDPTCOMPLETE                   *(volatile U32*)(USB0_BASE_ADDR + 0x000001BC) // Endpoint Tx Complete (Device mode)
#define REG_USB0_ENDPTCTRL0                      *(volatile U32*)(USB0_BASE_ADDR + 0x000001C0) // Endpoint 0 (Device mode)
#define REG_USB0_ENDPTCTRL1                      *(volatile U32*)(USB0_BASE_ADDR + 0x000001C4) // Endpoints 1 to 11 (Device mode)
#define REG_USB0_ENDPTCTRL2                      *(volatile U32*)(USB0_BASE_ADDR + 0x000001C8) // Endpoints 1 to 11 (Device mode)
#define REG_USB0_ENDPTCTRL3                      *(volatile U32*)(USB0_BASE_ADDR + 0x000001CC) // Endpoints 1 to 11 (Device mode)
#define REG_USB0_ENDPTCTRL4                      *(volatile U32*)(USB0_BASE_ADDR + 0x000001D0) // Endpoints 1 to 11 (Device mode)
#define REG_USB0_ENDPTCTRL5                      *(volatile U32*)(USB0_BASE_ADDR + 0x000001D4) // Endpoints 1 to 11 (Device mode)
#define REG_USB0_ENDPTCTRL6                      *(volatile U32*)(USB0_BASE_ADDR + 0x000001D8) // Endpoints 1 to 11 (Device mode)
#define REG_USB0_ENDPTCTRL7                      *(volatile U32*)(USB0_BASE_ADDR + 0x000001DC) // Endpoints 1 to 11 (Device mode)
#define REG_USB0_ENDPTCTRL8                      *(volatile U32*)(USB0_BASE_ADDR + 0x000001E0) // Endpoints 1 to 11 (Device mode)
#define REG_USB0_ENDPTCTRL9                      *(volatile U32*)(USB0_BASE_ADDR + 0x000001E4) // Endpoints 1 to 11 (Device mode)
#define REG_USB0_ENDPTCTRL10                     *(volatile U32*)(USB0_BASE_ADDR + 0x000001E8) // Endpoints 1 to 11 (Device mode)
#define REG_USB0_ENDPTCTRL11                     *(volatile U32*)(USB0_BASE_ADDR + 0x000001EC) // Endpoints 1 to 11 (Device mode)

#define USB1_BASE_ADDR                         (0xE0003000)
#define REG_USB1_ID                             *(volatile U32*)(USB1_BASE_ADDR + 0x00000000) // IP version and revision, read-only
#define REG_USB1_HWGENERAL                      *(volatile U32*)(USB1_BASE_ADDR + 0x00000004) // Misc IP config constants, read-only
#define REG_USB1_HWHOST                         *(volatile U32*)(USB1_BASE_ADDR + 0x00000008) // Host Mode IP config constants, read-only
#define REG_USB1_HWDEVICE                       *(volatile U32*)(USB1_BASE_ADDR + 0x0000000C) // Device Mode IP config constants, read-only
#define REG_USB1_HWTXBUF                        *(volatile U32*)(USB1_BASE_ADDR + 0x00000010) // TxBuffer IP config constants, read-only
#define REG_USB1_HWRXBUF                        *(volatile U32*)(USB1_BASE_ADDR + 0x00000014) // IP constants, RX buffer constants, read-only
#define REG_USB1_GPTIMER0LD                     *(volatile U32*)(USB1_BASE_ADDR + 0x00000080) // GP Timer 0 Load Value
#define REG_USB1_GPTIMER0CTRL                   *(volatile U32*)(USB1_BASE_ADDR + 0x00000084) // GP Timer 1 Control
#define REG_USB1_GPTIMER1LD                     *(volatile U32*)(USB1_BASE_ADDR + 0x00000088) // GP Timer 1 Load Value
#define REG_USB1_GPTIMER1CTRL                   *(volatile U32*)(USB1_BASE_ADDR + 0x0000008C) // GP Timer 1 Control
#define REG_USB1_SBUSCFG                        *(volatile U32*)(USB1_BASE_ADDR + 0x00000090) // DMA Master AHB Burst Mode
#define REG_USB1_CAPLENGTH_HCIVERSION           *(volatile U32*)(USB1_BASE_ADDR + 0x00000100) // EHCI Addr Space and HCI constants, read-only
#define REG_USB1_HCSPARAMS                      *(volatile U32*)(USB1_BASE_ADDR + 0x00000104) // TT counts and EHCI HCS constants, read-only
#define REG_USB1_HCCPARAMS                      *(volatile U32*)(USB1_BASE_ADDR + 0x00000108) // EHCI Host configuration constants
#define REG_USB1_DCIVERSION                     *(volatile U32*)(USB1_BASE_ADDR + 0x00000120) // Device Mode CI version constant
#define REG_USB1_DCCPARAMS                      *(volatile U32*)(USB1_BASE_ADDR + 0x00000124) // EHCI, device and endpoint capabilities
#define REG_USB1_USBCMD                         *(volatile U32*)(USB1_BASE_ADDR + 0x00000140) // USB Commands (EHCI extended)
#define REG_USB1_USBSTS                         *(volatile U32*)(USB1_BASE_ADDR + 0x00000144) // Interrupt/Raw Status (EHCI extended) (Host/Device)
#define REG_USB1_USBINTR                        *(volatile U32*)(USB1_BASE_ADDR + 0x00000148) // Interrrupts and Enables
#define REG_USB1_FRINDEX                        *(volatile U32*)(USB1_BASE_ADDR + 0x0000014C) // Frame List Index
#define REG_USB1_PERIODICLISTBASE_DEVICEADDR    *(volatile U32*)(USB1_BASE_ADDR + 0x00000154) // Host/Device Address dual-use
#define REG_USB1_ASYNCLISTADDR_ENDPOINTLISTADDR *(volatile U32*)(USB1_BASE_ADDR + 0x00000158) // Host/Device dual-use
#define REG_USB1_TTCTRL                         *(volatile U32*)(USB1_BASE_ADDR + 0x0000015C) // TT Control
#define REG_USB1_BURSTSIZE                      *(volatile U32*)(USB1_BASE_ADDR + 0x00000160) // Burst Size
#define REG_USB1_TXFILLTUNING                   *(volatile U32*)(USB1_BASE_ADDR + 0x00000164) // TxFIFO Fill Tuning
#define REG_USB1_TXTTFILLTUNING                 *(volatile U32*)(USB1_BASE_ADDR + 0x00000168) // TT TX latency FIFO
#define REG_USB1_IC_USB                         *(volatile U32*)(USB1_BASE_ADDR + 0x0000016C) // Low and Fast Speed Control constants
#define REG_USB1_ULPI_VIEWPORT                  *(volatile U32*)(USB1_BASE_ADDR + 0x00000170) // ULPI Viewport
#define REG_USB1_ENDPTNAK                       *(volatile U32*)(USB1_BASE_ADDR + 0x00000178) // Endpoint NAK (Device mode)
#define REG_USB1_ENDPTNAKEN                     *(volatile U32*)(USB1_BASE_ADDR + 0x0000017C) // Endpoint NAK (Device mode)
#define REG_USB1_CONFIGFLAG                     *(volatile U32*)(USB1_BASE_ADDR + 0x00000180) // reserved
#define REG_USB1_PORTSC1                        *(volatile U32*)(USB1_BASE_ADDR + 0x00000184) // Port Status & Control
#define REG_USB1_OTGSC                          *(volatile U32*)(USB1_BASE_ADDR + 0x000001A4) // OTG Status and Control
#define REG_USB1_USBMODE                        *(volatile U32*)(USB1_BASE_ADDR + 0x000001A8) // USB Mode Selection
#define REG_USB1_ENDPTSETUPSTAT                 *(volatile U32*)(USB1_BASE_ADDR + 0x000001AC) // Endpoint Status Setup (Device mode)
#define REG_USB1_ENDPTPRIME                     *(volatile U32*)(USB1_BASE_ADDR + 0x000001B0) // Endpoint Primer (Device mode)
#define REG_USB1_ENDPTFLUSH                     *(volatile U32*)(USB1_BASE_ADDR + 0x000001B4) // Endpoint Flush (Device mode)
#define REG_USB1_ENDPTSTAT                      *(volatile U32*)(USB1_BASE_ADDR + 0x000001B8) // Endpoint Buffer Ready Status (Device mode), RO
#define REG_USB1_ENDPTCOMPLETE                  *(volatile U32*)(USB1_BASE_ADDR + 0x000001BC) // Endpoint Tx Complete (Device mode)
#define REG_USB1_ENDPTCTRL0                     *(volatile U32*)(USB1_BASE_ADDR + 0x000001C0) // Endpoint 0 (Device mode)
#define REG_USB1_ENDPTCTRL1                     *(volatile U32*)(USB1_BASE_ADDR + 0x000001C4) // Endpoints 1 to 11 (Device mode)
#define REG_USB1_ENDPTCTRL2                     *(volatile U32*)(USB1_BASE_ADDR + 0x000001C8) // Endpoints 1 to 11 (Device mode)
#define REG_USB1_ENDPTCTRL3                     *(volatile U32*)(USB1_BASE_ADDR + 0x000001CC) // Endpoints 1 to 11 (Device mode)
#define REG_USB1_ENDPTCTRL4                     *(volatile U32*)(USB1_BASE_ADDR + 0x000001D0) // Endpoints 1 to 11 (Device mode)
#define REG_USB1_ENDPTCTRL5                     *(volatile U32*)(USB1_BASE_ADDR + 0x000001D4) // Endpoints 1 to 11 (Device mode)
#define REG_USB1_ENDPTCTRL6                     *(volatile U32*)(USB1_BASE_ADDR + 0x000001D8) // Endpoints 1 to 11 (Device mode)
#define REG_USB1_ENDPTCTRL7                     *(volatile U32*)(USB1_BASE_ADDR + 0x000001DC) // Endpoints 1 to 11 (Device mode)
#define REG_USB1_ENDPTCTRL8                     *(volatile U32*)(USB1_BASE_ADDR + 0x000001E0) // Endpoints 1 to 11 (Device mode)
#define REG_USB1_ENDPTCTRL9                     *(volatile U32*)(USB1_BASE_ADDR + 0x000001E4) // Endpoints 1 to 11 (Device mode)
#define REG_USB1_ENDPTCTRL10                    *(volatile U32*)(USB1_BASE_ADDR + 0x000001E8) // Endpoints 1 to 11 (Device mode)
#define REG_USB1_ENDPTCTRL11                    *(volatile U32*)(USB1_BASE_ADDR + 0x000001EC) // Endpoints 1 to 11 (Device mode)

/****** UART registers *****************************************/
#define UART0_BASE_ADDR 0xE0000000
#define UART1_BASE_ADDR 0xE0001000

#define REG_UART0_CR               *(volatile U32*)(UART0_BASE_ADDR + 0x00000000)
#define REG_UART0_MR               *(volatile U32*)(UART0_BASE_ADDR + 0x00000004)
#define REG_UART0_IER              *(volatile U32*)(UART0_BASE_ADDR + 0x00000008)
#define REG_UART0_IDR              *(volatile U32*)(UART0_BASE_ADDR + 0x0000000C)
#define REG_UART0_IMR              *(volatile U32*)(UART0_BASE_ADDR + 0x00000010)
#define REG_UART0_ISR              *(volatile U32*)(UART0_BASE_ADDR + 0x00000014)
#define REG_UART0_BAUDGEN          *(volatile U32*)(UART0_BASE_ADDR + 0x00000018)
#define REG_UART0_RXTOUT           *(volatile U32*)(UART0_BASE_ADDR + 0x0000001C)
#define REG_UART0_RXWM             *(volatile U32*)(UART0_BASE_ADDR + 0x00000020)
#define REG_UART0_MODEMCR          *(volatile U32*)(UART0_BASE_ADDR + 0x00000024)
#define REG_UART0_MODEMSR          *(volatile U32*)(UART0_BASE_ADDR + 0x00000028)
#define REG_UART0_SR               *(volatile U32*)(UART0_BASE_ADDR + 0x0000002C)
#define REG_UART0_RXFIFO           *(volatile U32*)(UART0_BASE_ADDR + 0x00000030)
#define REG_UART0_BDIV             *(volatile U32*)(UART0_BASE_ADDR + 0x00000034)
#define REG_UART0_FDR              *(volatile U32*)(UART0_BASE_ADDR + 0x00000038)
#define REG_UART0_TXFIFO_TRGLVL    *(volatile U32*)(UART0_BASE_ADDR + 0x00000044)

/****** Static Memory Controller *****************************************/
#define MEMC_BASE_ADDR          0xE0000000
#define REG_MEMC_CFG_SET        *(volatile U32*)(MEMC_BASE_ADDR + 0xE008)

///****** Interrupt ID Table **********************************************/
//typedef enum IRQn
//{
//  XADC_IRQn                   = 39,   // ADC Interrupt
//  SWDT_IRQn                   = 41   // System Watchdog Interrupt
//} IRQn_Type;

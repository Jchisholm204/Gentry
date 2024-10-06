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

-------------------------- END-OF-HEADER -----------------------------
Purpose : SFR definitions for the Zynq7007S.

*/

/****** FPGA Base Address (defined in Xilinx Vivado Tool) ***********/
#define HW_FPGA_REG_BASE_ADDR                            (0x43C00000u)

/****** MIO, EMIO Configuration *************************************/
#define MIO_BASE_ADDR                                    (0xE000A000u)
#define REG_MIO_BANK0_DATA_RO                            (*(volatile unsigned long*)(MIO_BASE_ADDR + 0x0060u))  // Input data
#define REG_MIO_MASK_DATA_0_LSW                          (*(volatile unsigned long*)(MIO_BASE_ADDR + 0x0000u))  // Maskable Output Data (GPIO Bank0, MIO, Lower 16bits)
#define REG_MIO_MASK_DATA_0_MSW                          (*(volatile unsigned long*)(MIO_BASE_ADDR + 0x0004u))  // Maskable Output Data (GPIO Bank0, MIO, Upper 16bits)
#define REG_MIO_MASK_DATA_1_LSW                          (*(volatile unsigned long*)(MIO_BASE_ADDR + 0x0008u))  // Maskable Output Data (GPIO Bank1, MIO, Lower 16bits)
#define REG_MIO_MASK_DATA_1_MSW                          (*(volatile unsigned long*)(MIO_BASE_ADDR + 0x000Cu))  // Maskable Output Data (GPIO Bank1, MIO, Upper 6bits)
#define REG_MIO_MASK_DATA_2_LSW                          (*(volatile unsigned long*)(MIO_BASE_ADDR + 0x0010u))  // Maskable Output Data (GPIO Bank2, EMIO, Lower 16bits)
#define REG_MIO_MASK_DATA_2_MSW                          (*(volatile unsigned long*)(MIO_BASE_ADDR + 0x0014u))  // Maskable Output Data (GPIO Bank2, EMIO, Upper 16bits)
#define REG_MIO_MASK_DATA_3_LSW                          (*(volatile unsigned long*)(MIO_BASE_ADDR + 0x0018u))  // Maskable Output Data (GPIO Bank3, EMIO, Lower 16bits)
#define REG_MIO_MASK_DATA_3_MSW                          (*(volatile unsigned long*)(MIO_BASE_ADDR + 0x001Cu))  // Maskable Output Data (GPIO Bank3, EMIO, Upper 16bits)
#define REG_MIO_DATA_0                                   (*(volatile unsigned long*)(MIO_BASE_ADDR + 0x0040u))  // Output Data (GPIO Bank0, MIO)
#define REG_MIO_DATA_1                                   (*(volatile unsigned long*)(MIO_BASE_ADDR + 0x0044u))  // Output Data (GPIO Bank1, MIO)
#define REG_MIO_DATA_2                                   (*(volatile unsigned long*)(MIO_BASE_ADDR + 0x0048u))  // Output Data (GPIO Bank2, EMIO)
#define REG_MIO_DATA_3                                   (*(volatile unsigned long*)(MIO_BASE_ADDR + 0x004Cu))  // Output Data (GPIO Bank3, EMIO)
#define REG_MIO_DATA_0_RO                                (*(volatile unsigned long*)(MIO_BASE_ADDR + 0x0060u))  // Input Data (GPIO Bank0, MIO)
#define REG_MIO_DATA_1_RO                                (*(volatile unsigned long*)(MIO_BASE_ADDR + 0x0064u))  // Input Data (GPIO Bank1, MIO)
#define REG_MIO_DATA_2_RO                                (*(volatile unsigned long*)(MIO_BASE_ADDR + 0x0068u))  // Input Data (GPIO Bank2, EMIO)
#define REG_MIO_DATA_3_RO                                (*(volatile unsigned long*)(MIO_BASE_ADDR + 0x006Cu))  // Input Data (GPIO Bank3, EMIO)
#define REG_MIO_DIRM_0                                   (*(volatile unsigned long*)(MIO_BASE_ADDR + 0x0204u))  // Direction mode (GPIO Bank0, MIO)
#define REG_MIO_OEN_0                                    (*(volatile unsigned long*)(MIO_BASE_ADDR + 0x0208u))  // Output enable (GPIO Bank0, MIO)
#define REG_MIO_INT_MASK_0                               (*(volatile unsigned long*)(MIO_BASE_ADDR + 0x020Cu))  // Interrupt Mask Status (GPIO Bank0, MIO)
#define REG_MIO_INT_EN_0                                 (*(volatile unsigned long*)(MIO_BASE_ADDR + 0x0210u))  // Interrupt Enable/Unmask (GPIO Bank0, MIO)
#define REG_MIO_INT_DIS_0                                (*(volatile unsigned long*)(MIO_BASE_ADDR + 0x0214u))  // Interrupt Disable/Mask (GPIO Bank0, MIO)
#define REG_MIO_INT_STAT_0                               (*(volatile unsigned long*)(MIO_BASE_ADDR + 0x0218u))  // Interrupt Status (GPIO Bank0, MIO)
#define REG_MIO_INT_TYPE_0                               (*(volatile unsigned long*)(MIO_BASE_ADDR + 0x021Cu))  // Interrupt Type (GPIO Bank0, MIO)
#define REG_MIO_INT_POLARITY_0                           (*(volatile unsigned long*)(MIO_BASE_ADDR + 0x0220u))  // Interrupt Polarity (GPIO Bank0, MIO)
#define REG_MIO_INT_ANY_0                                (*(volatile unsigned long*)(MIO_BASE_ADDR + 0x0224u))  // Interrupt Any Edge Sensitive (GPIO Bank0, MIO)
#define REG_MIO_DIRM_1                                   (*(volatile unsigned long*)(MIO_BASE_ADDR + 0x0244u))  // Direction mode (GPIO Bank1, MIO)
#define REG_MIO_OEN_1                                    (*(volatile unsigned long*)(MIO_BASE_ADDR + 0x0248u))  // Output enable (GPIO Bank1, MIO)
#define REG_MIO_INT_MASK_1                               (*(volatile unsigned long*)(MIO_BASE_ADDR + 0x024Cu))  // Interrupt Mask Status (GPIO Bank1, MIO)
#define REG_MIO_INT_EN_1                                 (*(volatile unsigned long*)(MIO_BASE_ADDR + 0x0250u))  // Interrupt Enable/Unmask (GPIO Bank1, MIO)
#define REG_MIO_INT_DIS_1                                (*(volatile unsigned long*)(MIO_BASE_ADDR + 0x0254u))  // Interrupt Disable/Mask (GPIO Bank1, MIO)
#define REG_MIO_INT_STAT_1                               (*(volatile unsigned long*)(MIO_BASE_ADDR + 0x0258u))  // Interrupt Status (GPIO Bank1, MIO)
#define REG_MIO_INT_TYPE_1                               (*(volatile unsigned long*)(MIO_BASE_ADDR + 0x025Cu))  // Interrupt Type (GPIO Bank1, MIO)
#define REG_MIO_INT_POLARITY_1                           (*(volatile unsigned long*)(MIO_BASE_ADDR + 0x0260u))  // Interrupt Polarity (GPIO Bank1, MIO)
#define REG_MIO_INT_ANY_1                                (*(volatile unsigned long*)(MIO_BASE_ADDR + 0x0264u))  // Interrupt Any Edge Sensitive (GPIO Bank1, MIO)
#define REG_MIO_DIRM_2                                   (*(volatile unsigned long*)(MIO_BASE_ADDR + 0x0284u))  // Direction mode (GPIO Bank2, EMIO)
#define REG_MIO_OEN_2                                    (*(volatile unsigned long*)(MIO_BASE_ADDR + 0x0288u))  // Output enable (GPIO Bank2, EMIO)
#define REG_MIO_INT_MASK_2                               (*(volatile unsigned long*)(MIO_BASE_ADDR + 0x028Cu))  // Interrupt Mask Status (GPIO Bank2, EMIO)
#define REG_MIO_INT_EN_2                                 (*(volatile unsigned long*)(MIO_BASE_ADDR + 0x0290u))  // Interrupt Enable/Unmask (GPIO Bank2, EMIO)
#define REG_MIO_INT_DIS_2                                (*(volatile unsigned long*)(MIO_BASE_ADDR + 0x0294u))  // Interrupt Disable/Mask (GPIO Bank2, EMIO)
#define REG_MIO_INT_STAT_2                               (*(volatile unsigned long*)(MIO_BASE_ADDR + 0x0298u))  // Interrupt Status (GPIO Bank2, EMIO)
#define REG_MIO_INT_TYPE_2                               (*(volatile unsigned long*)(MIO_BASE_ADDR + 0x029Cu))  // Interrupt Type (GPIO Bank2, EMIO)
#define REG_MIO_INT_POLARITY_2                           (*(volatile unsigned long*)(MIO_BASE_ADDR + 0x02A0u))  // Interrupt Polarity (GPIO Bank2, EMIO)
#define REG_MIO_INT_ANY_2                                (*(volatile unsigned long*)(MIO_BASE_ADDR + 0x02A4u))  // Interrupt Any Edge Sensitive (GPIO Bank2, EMIO)
#define REG_MIO_DIRM_3                                   (*(volatile unsigned long*)(MIO_BASE_ADDR + 0x02C4u))  // Direction mode (GPIO Bank3, EMIO)
#define REG_MIO_OEN_3                                    (*(volatile unsigned long*)(MIO_BASE_ADDR + 0x02C8u))  // Output enable (GPIO Bank3, EMIO)
#define REG_MIO_INT_MASK_3                               (*(volatile unsigned long*)(MIO_BASE_ADDR + 0x02CCu))  // Interrupt Mask Status (GPIO Bank3, EMIO)
#define REG_MIO_INT_EN_3                                 (*(volatile unsigned long*)(MIO_BASE_ADDR + 0x02D0u))  // Interrupt Enable/Unmask (GPIO Bank3, EMIO)
#define REG_MIO_INT_DIS_3                                (*(volatile unsigned long*)(MIO_BASE_ADDR + 0x02D4u))  // Interrupt Disable/Mask (GPIO Bank3, EMIO)
#define REG_MIO_INT_STAT_3                               (*(volatile unsigned long*)(MIO_BASE_ADDR + 0x02D8u))  // Interrupt Status (GPIO Bank3, EMIO)
#define REG_MIO_INT_TYPE_3                               (*(volatile unsigned long*)(MIO_BASE_ADDR + 0x02DCu))  // Interrupt Type (GPIO Bank3, EMIO)
#define REG_MIO_INT_POLARITY_3                           (*(volatile unsigned long*)(MIO_BASE_ADDR + 0x02E0u))  // Interrupt Polarity (GPIO Bank3, EMIO)
#define REG_MIO_INT_ANY_3                                (*(volatile unsigned long*)(MIO_BASE_ADDR + 0x02E4u))  // Interrupt Any Edge Sensitive (GPIO Bank3, EMIO)

/****** DDR Controller **********************************************/
#define DDRC_BASE_ADDR                                   (0xF8006000u)
#define REG_DDRC_CTRL                                    (*(volatile unsigned long*)(0xF8006000u))              // DDRC Control
#define REG_DDRC_TWO_RANK_CFG                            (*(volatile unsigned long*)(0xF8006004u))              // Two Rank Configuration
#define REG_DDRC_HPR_REG                                 (*(volatile unsigned long*)(0xF8006008u))              // HPR Queue control
#define REG_DDRC_LPR_REG                                 (*(volatile unsigned long*)(0xF800600Cu))              // LPR Queue control
#define REG_DDRC_WR_REG                                  (*(volatile unsigned long*)(0xF8006010u))              // WR Queue control
#define REG_DDRC_DRAM_PARAM_REG0                         (*(volatile unsigned long*)(0xF8006014u))              // DRAM Parameters 0
#define REG_DDRC_DRAM_PARAM_REG1                         (*(volatile unsigned long*)(0xF8006018u))              // DRAM Parameters 1
#define REG_DDRC_DRAM_PARAM_REG2                         (*(volatile unsigned long*)(0xF800601Cu))              // DRAM Parameters 2
#define REG_DDRC_DRAM_PARAM_REG3                         (*(volatile unsigned long*)(0xF8006020u))              // DRAM Parameters 3
#define REG_DDRC_DRAM_PARAM_REG4                         (*(volatile unsigned long*)(0xF8006024u))              // DRAM Parameters 4
#define REG_DDRC_DRAM_INIT_PARAM                         (*(volatile unsigned long*)(0xF8006028u))              // DRAM Initialization Parameters
#define REG_DDRC_DRAM_EMR_REG                            (*(volatile unsigned long*)(0xF800602Cu))              // DRAM EMR2, EMR3 access
#define REG_DDRC_DRAM_EMR_MR_REG                         (*(volatile unsigned long*)(0xF8006030u))              // DRAM EMR, MR access
#define REG_DDRC_DRAM_BURST8_RDWR                        (*(volatile unsigned long*)(0xF8006034u))              // DRAM Burst 8 read/write
#define REG_DDRC_DRAM_DISABLE_DQ                         (*(volatile unsigned long*)(0xF8006038u))              // DRAM Disable DQDRAM_addr_map_bank
#define REG_DDRC_DRAM_ADDR_MAP_BANK                      (*(volatile unsigned long*)(0xF800603Cu))              // Row/Column address bits
#define REG_DDRC_DRAM_ADDR_MAP_COL                       (*(volatile unsigned long*)(0xF8006040u))              // Column address bits
#define REG_DDRC_DRAM_ADDR_MAP_ROW                       (*(volatile unsigned long*)(0xF8006044u))              // Select DRAM row address bits
#define REG_DDRC_DRAM_ODT_REG                            (*(volatile unsigned long*)(0xF8006048u))              // DRAM ODT control
#define REG_DDRC_PHY_DBG_REG                             (*(volatile unsigned long*)(0xF800604Cu))              // PHY debug
#define REG_DDRC_PHY_CMD_TIMEOUT_RDD_ATA_CPT             (*(volatile unsigned long*)(0xF8006050u))              // PHY command time out and read data capture FIFO
#define REG_DDRC_MODE_STS_REG                            (*(volatile unsigned long*)(0xF8006054u))              // Controller operation mode status
#define REG_DDRC_DLL_CALIB                               (*(volatile unsigned long*)(0xF8006058u))              // DLL calibration
#define REG_DDRC_ODT_DELAY_HOLD                          (*(volatile unsigned long*)(0xF800605Cu))              // ODT delay and ODT hold
#define REG_DDRC_CTRL_REG1                               (*(volatile unsigned long*)(0xF8006060u))              // Controller 1
#define REG_DDRC_CTRL_REG2                               (*(volatile unsigned long*)(0xF8006064u))              // Controller 2
#define REG_DDRC_CTRL_REG3                               (*(volatile unsigned long*)(0xF8006068u))              // Controller 3
#define REG_DDRC_CTRL_REG4                               (*(volatile unsigned long*)(0xF800606Cu))              // Controller 4
#define REG_DDRC_CTRL_REG5                               (*(volatile unsigned long*)(0xF8006078u))              // Controller register 5
#define REG_DDRC_CTRL_REG6                               (*(volatile unsigned long*)(0xF800607Cu))              // Controller register 6
#define REG_DDRC_CHE_REFRESH_TIMER01                     (*(volatile unsigned long*)(0xF80060A0u))              // CHE_REFRESH_TIMER01
#define REG_DDRC_CHE_T_ZQ                                (*(volatile unsigned long*)(0xF80060A4u))              // ZQ parameters
#define REG_DDRC_CHE_T_ZQ_SHORT_INTERVAL_REG             (*(volatile unsigned long*)(0xF80060A8u))              // Misc parameters
#define REG_DDRC_DEEP_PWRDWN_REG                         (*(volatile unsigned long*)(0xF80060ACu))              // Deep powerdown (LPDDR2)
#define REG_DDRC_REG_2C                                  (*(volatile unsigned long*)(0xF80060B0u))              // Training control
#define REG_DDRC_REG_2D                                  (*(volatile unsigned long*)(0xF80060B4u))              // Misc Debug
#define REG_DDRC_DFI_TIMING                              (*(volatile unsigned long*)(0xF80060B8u))              // DFI timing
#define REG_DDRC_CHE_ECC_CONTROL_REG_OFFSET              (*(volatile unsigned long*)(0xF80060C4u))              // ECC error clear
#define REG_DDRC_CHE_CORR_ECC_LOG_REG_OFFSET             (*(volatile unsigned long*)(0xF80060C8u))              // ECC error correction
#define REG_DDRC_CHE_CORR_ECC_ADDR_REG_OFFSET            (*(volatile unsigned long*)(0xF80060CCu))              // ECC error correction address log
#define REG_DDRC_CHE_CORR_ECC_DATA_31_0_REG_OFFSET       (*(volatile unsigned long*)(0xF80060D0u))              // ECC error correction data log low
#define REG_DDRC_CHE_CORR_ECC_DATA_63_32_REG_OFFSET      (*(volatile unsigned long*)(0xF80060D4u))              // ECC error correction data log mid
#define REG_DDRC_CHE_CORR_ECC_DATA_71_64_REG_OFFSET      (*(volatile unsigned long*)(0xF80060D8u))              // ECC error correction data log high
#define REG_DDRC_CHE_UNCORR_ECC_LOG_REG_OFFSET           (*(volatile unsigned long*)(0xF80060DCu))              // ECC unrecoverable error status
#define REG_DDRC_CHE_UNCORR_ECC_ADDR_REG_OFFSET          (*(volatile unsigned long*)(0xF80060E0u))              // ECC unrecoverable error address
#define REG_DDRC_CHE_UNCORR_ECC_DATA_31_0_REG_OFFSET     (*(volatile unsigned long*)(0xF80060E4u))              // ECC unrecoverable error data low
#define REG_DDRC_CHE_UNCORR_ECC_DATA_63_32_REG_OFFSET    (*(volatile unsigned long*)(0xF80060E8u))              // ECC unrecoverable error data middle
#define REG_DDRC_CHE_UNCORR_ECC_DATA_71_64_REG_OFFSET    (*(volatile unsigned long*)(0xF80060ECu))              // ECC unrecoverable error data high
#define REG_DDRC_CHE_ECC_STATS_REG_OFFSET                (*(volatile unsigned long*)(0xF80060F0u))              // ECC error count
#define REG_DDRC_ECC_SCRUB                               (*(volatile unsigned long*)(0xF80060F4u))              // ECC mode/scrub
#define REG_DDRC_CHE_ECC_CORR_BIT_MASK_31_0_REG_OFFSET   (*(volatile unsigned long*)(0xF80060F8u))              // ECC data mask low
#define REG_DDRC_CHE_ECC_CORR_BIT_MASK_63_32_REG_OFFSET  (*(volatile unsigned long*)(0xF80060FCu))              // ECC data mask high
#define REG_DDRC_PHY_RCVR_ENABLE                         (*(volatile unsigned long*)(0xF8006114u))              // Phy receiver enable register
#define REG_DDRC_PHY_CONFIG0                             (*(volatile unsigned long*)(0xF8006118u))              // PHY configuration register for data slice 0.
#define REG_DDRC_PHY_CONFIG1                             (*(volatile unsigned long*)(0xF800611Cu))              // PHY configuration register for data slice 1.
#define REG_DDRC_PHY_CONFIG2                             (*(volatile unsigned long*)(0xF8006120u))              // PHY configuration register for data slice 2.
#define REG_DDRC_PHY_CONFIG3                             (*(volatile unsigned long*)(0xF8006124u))              // PHY configuration register for data slice 3.
#define REG_DDRC_PHY_INIT_RATIO0                         (*(volatile unsigned long*)(0xF800612Cu))              // PHY init ratio register for data slice 0.
#define REG_DDRC_PHY_INIT_RATIO1                         (*(volatile unsigned long*)(0xF8006130u))              // PHY init ratio register for data slice 1.
#define REG_DDRC_PHY_INIT_RATIO2                         (*(volatile unsigned long*)(0xF8006134u))              // PHY init ratio register for data slice 2.
#define REG_DDRC_PHY_INIT_RATIO3                         (*(volatile unsigned long*)(0xF8006138u))              // PHY init ratio register for data slice 3.
#define REG_DDRC_PHY_RD_DQS_CFG0                         (*(volatile unsigned long*)(0xF8006140u))              // PHY read DQS configuration register for data slice 0.
#define REG_DDRC_PHY_RD_DQS_CFG1                         (*(volatile unsigned long*)(0xF8006144u))              // PHY read DQS configuration register for data slice 1.
#define REG_DDRC_PHY_RD_DQS_CFG2                         (*(volatile unsigned long*)(0xF8006148u))              // PHY read DQS configuration register for data slice 2.
#define REG_DDRC_PHY_RD_DQS_CFG3                         (*(volatile unsigned long*)(0xF800614Cu))              // PHY read DQS configuration register for data slice 3.
#define REG_DDRC_PHY_WR_DQS_CFG0                         (*(volatile unsigned long*)(0xF8006154u))              // PHY write DQS configuration register for data slice 0.
#define REG_DDRC_PHY_WR_DQS_CFG1                         (*(volatile unsigned long*)(0xF8006158u))              // PHY write DQS configuration register for data slice 1.
#define REG_DDRC_PHY_WR_DQS_CFG2                         (*(volatile unsigned long*)(0xF800615Cu))              // PHY write DQS configuration register for data slice 2.
#define REG_DDRC_PHY_WR_DQS_CFG3                         (*(volatile unsigned long*)(0xF8006160u))              // PHY write DQS configuration register for data slice 3.
#define REG_DDRC_PHY_WE_CFG0                             (*(volatile unsigned long*)(0xF8006168u))              // PHY FIFO write enable configuration for data slice 0.
#define REG_DDRC_PHY_WE_CFG1                             (*(volatile unsigned long*)(0xF800616Cu))              // PHY FIFO write enable configuration for data slice 1.
#define REG_DDRC_PHY_WE_CFG2                             (*(volatile unsigned long*)(0xF8006170u))              // PHY FIFO write enable configuration for data slice 2.
#define REG_DDRC_PHY_WE_CFG3                             (*(volatile unsigned long*)(0xF8006174u))              // PHY FIFO write enable configuration for data slice 3.
#define REG_DDRC_WR_DATA_SLV0                            (*(volatile unsigned long*)(0xF800617Cu))              // PHY write data slave ratio config for data slice 0.
#define REG_DDRC_WR_DATA_SLV1                            (*(volatile unsigned long*)(0xF8006180u))              // PHY write data slave ratio config for data slice 1.
#define REG_DDRC_WR_DATA_SLV2                            (*(volatile unsigned long*)(0xF8006184u))              // PHY write data slave ratio config for data slice 2.
#define REG_DDRC_WR_DATA_SLV3                            (*(volatile unsigned long*)(0xF8006188u))              // PHY write data slave ratio config for data slice 3.
#define REG_DDRC_REG_64                                  (*(volatile unsigned long*)(0xF8006190u))              // Training control 2
#define REG_DDRC_REG_65                                  (*(volatile unsigned long*)(0xF8006194u))              // Training control 3
#define REG_DDRC_REG69_6A0                               (*(volatile unsigned long*)(0xF80061A4u))              // Training results for data slice 0.
#define REG_DDRC_REG69_6A1                               (*(volatile unsigned long*)(0xF80061A8u))              // Training results for data slice 1.
#define REG_DDRC_REG6C_6D2                               (*(volatile unsigned long*)(0xF80061B0u))              // Training results for data slice 2.
#define REG_DDRC_REG6C_6D3                               (*(volatile unsigned long*)(0xF80061B4u))              // Training results for data slice 3.
#define REG_DDRC_REG6E_710                               (*(volatile unsigned long*)(0xF80061B8u))              // Training results (2) for data slice 0.
#define REG_DDRC_REG6E_711                               (*(volatile unsigned long*)(0xF80061BCu))              // Training results (2) for data slice 1.
#define REG_DDRC_REG6E_712                               (*(volatile unsigned long*)(0xF80061C0u))              // Training results (2) for data slice 2.
#define REG_DDRC_REG6E_713                               (*(volatile unsigned long*)(0xF80061C4u))              // Training results (2) for data slice 3.
#define REG_DDRC_PHY_DLL_STS0                            (*(volatile unsigned long*)(0xF80061CCu))              // Slave DLL results for data slice 0.
#define REG_DDRC_PHY_DLL_STS1                            (*(volatile unsigned long*)(0xF80061D0u))              // Slave DLL results for data slice 1.
#define REG_DDRC_PHY_DLL_STS2                            (*(volatile unsigned long*)(0xF80061D4u))              // Slave DLL results for data slice 2.
#define REG_DDRC_PHY_DLL_STS3                            (*(volatile unsigned long*)(0xF80061D8u))              // Slave DLL results for data slice 3.
#define REG_DDRC_DLL_LOCK_STS                            (*(volatile unsigned long*)(0xF80061E0u))              // DLL Lock Status, read
#define REG_DDRC_PHY_CTRL_STS                            (*(volatile unsigned long*)(0xF80061E4u))              // PHY Control status, read
#define REG_DDRC_PHY_CTRL_STS_REG2                       (*(volatile unsigned long*)(0xF80061E8u))              // PHY Control status (2), read
#define REG_DDRC_AXI_ID                                  (*(volatile unsigned long*)(0xF8006200u))              // ID and revision information
#define REG_DDRC_PAGE_MASK                               (*(volatile unsigned long*)(0xF8006204u))              // Page mask
#define REG_DDRC_AXI_PRIORITY_WR_PORT0                   (*(volatile unsigned long*)(0xF8006208u))              // AXI Priority control for write port 0.
#define REG_DDRC_AXI_PRIORITY_WR_PORT1                   (*(volatile unsigned long*)(0xF800620Cu))              // AXI Priority control for write port 1.
#define REG_DDRC_AXI_PRIORITY_WR_PORT2                   (*(volatile unsigned long*)(0xF8006210u))              // AXI Priority control for write port 2.
#define REG_DDRC_AXI_PRIORITY_WR_PORT3                   (*(volatile unsigned long*)(0xF8006214u))              // AXI Priority control for write port 3.
#define REG_DDRC_AXI_PRIORITY_RD_PORT0                   (*(volatile unsigned long*)(0xF8006218u))              // AXI Priority control for read port 0.
#define REG_DDRC_AXI_PRIORITY_RD_PORT1                   (*(volatile unsigned long*)(0xF800621Cu))              // AXI Priority control for read port 1.
#define REG_DDRC_AXI_PRIORITY_RD_PORT2                   (*(volatile unsigned long*)(0xF8006220u))              // AXI Priority control for read port 2.
#define REG_DDRC_AXI_PRIORITY_RD_PORT3                   (*(volatile unsigned long*)(0xF8006224u))              // AXI Priority control for read port 3.
#define REG_DDRC_EXCL_ACCESS_CFG0                        (*(volatile unsigned long*)(0xF8006294u))              // Exclusive access configuration for port 0.
#define REG_DDRC_EXCL_ACCESS_CFG1                        (*(volatile unsigned long*)(0xF8006298u))              // Exclusive access configuration for port 1.
#define REG_DDRC_EXCL_ACCESS_CFG2                        (*(volatile unsigned long*)(0xF800629Cu))              // Exclusive access configuration for port 2.
#define REG_DDRC_EXCL_ACCESS_CFG3                        (*(volatile unsigned long*)(0xF80062A0u))              // Exclusive access configuration for port 3.
#define REG_DDRC_MODE_REG_READ                           (*(volatile unsigned long*)(0xF80062A4u))              // Mode register read data
#define REG_DDRC_LPDDR_CTRL0                             (*(volatile unsigned long*)(0xF80062A8u))              // LPDDR2 Control 0
#define REG_DDRC_LPDDR_CTRL1                             (*(volatile unsigned long*)(0xF80062ACu))              // LPDDR2 Control 1
#define REG_DDRC_LPDDR_CTRL2                             (*(volatile unsigned long*)(0xF80062B0u))              // LPDDR2 Control 2
#define REG_DDRC_LPDDR_CTRL3                             (*(volatile unsigned long*)(0xF80062B4u))              // LPDDR2 Control 3

/****** System Level Control *************************************/
#define MIO_PIN_OPT_DIS_RECV                             (1u << 13)
#define MIO_PIN_OPT_ENA_PULLUP                           (1u << 12)
#define MIO_PIN_OPT_IO_CMOS_3V3                          (3u <<  9)
#define MIO_PIN_OPT_IO_CMOS_2V5                          (2u <<  9)
#define MIO_PIN_OPT_IO_CMOS_1V8                          (1u <<  9)
#define MIO_PIN_OPT_IO_HSTL                              (4u <<  9)
#define MIO_PIN_OPT_ENA_SPEED_FAST                       (1u <<  8)
#define MIO_PIN_OPT_LVL30_MUX                            (0u <<  5)
#define MIO_PIN_OPT_LVL31_MUX                            (1u <<  5)
#define MIO_PIN_OPT_LVL32_MUX                            (2u <<  5)
#define MIO_PIN_OPT_LVL33_MUX                            (3u <<  5)
#define MIO_PIN_OPT_LVL34_MUX                            (4u <<  5)
#define MIO_PIN_OPT_LVL35_MUX                            (5u <<  5)
#define MIO_PIN_OPT_LVL36_MUX                            (6u <<  5)
#define MIO_PIN_OPT_LVL37_MUX                            (7u <<  5)
#define MIO_PIN_OPT_LVL23_MUX                            (3u <<  3)
#define MIO_PIN_OPT_LVL22_MUX                            (2u <<  3)
#define MIO_PIN_OPT_LVL21_MUX                            (1u <<  3)
#define MIO_PIN_OPT_LVL11_MUX                            (1u <<  2)
#define MIO_PIN_OPT_LVL01_MUX                            (1u <<  1)
#define MIO_PIN_OPT_ENA_TRISATE                          (1u <<  0)

#define SLCR_UNLOCK_KEY                                  (0xDF0Du)                                                 // System-level control unlock key
#define SLCR_LOCK_KEY                                    (0x767Bu)                                                 // System-level control lock key

#define SLCR_BASE_ADDR                                   (0xF8000000u)
#define REG_SLCR_LOCK                                    (*(volatile unsigned long*)(SLCR_BASE_ADDR + 0x0004u))    // SLCR lock register
#define REG_SLCR_UNLOCK                                  (*(volatile unsigned long*)(SLCR_BASE_ADDR + 0x0008u))    // SLCR unlock register
#define REG_SLCR_ARM_PLL_CTRL                            (*(volatile unsigned long*)(SLCR_BASE_ADDR + 0x0100u))    // ARM PLL Control
#define REG_SLCR_DDR_PLL_CTRL                            (*(volatile unsigned long*)(SLCR_BASE_ADDR + 0x0104u))    // DDR PLL Control
#define REG_SLCR_IO_PLL_CTRL                             (*(volatile unsigned long*)(SLCR_BASE_ADDR + 0x0108u))    // IO PLL Control
#define REG_SLCR_PLL_STATUS                              (*(volatile unsigned long*)(SLCR_BASE_ADDR + 0x010Cu))    // PLL Status
#define REG_SLCR_ARM_PLL_CFG                             (*(volatile unsigned long*)(SLCR_BASE_ADDR + 0x0110u))    // ARM PLL Configuration
#define REG_SLCR_DDR_PLL_CFG                             (*(volatile unsigned long*)(SLCR_BASE_ADDR + 0x0114u))    // DDR PLL Configuration
#define REG_SLCR_IO_PLL_CFG                              (*(volatile unsigned long*)(SLCR_BASE_ADDR + 0x0118u))    // IO PLL Configuration
#define REG_SLCR_ARM_CLK_CTRL                            (*(volatile unsigned long*)(SLCR_BASE_ADDR + 0x0120u))    // CPU Clock Control
#define REG_SLCR_DDR_CLK_CTRL                            (*(volatile unsigned long*)(SLCR_BASE_ADDR + 0x0124u))    // DDR Clock Control
#define REG_SLCR_DCI_CLK_CTRL                            (*(volatile unsigned long*)(SLCR_BASE_ADDR + 0x0128u))     // DCI clock control
#define REG_SLCR_USB0_CLK_CTRL                           (*(volatile unsigned long*)(SLCR_BASE_ADDR + 0x0130u))    // USB 0 ULPI Clock Control
#define REG_SLCR_USB1_CLK_CTRL                           (*(volatile unsigned long*)(SLCR_BASE_ADDR + 0x0134u))    // USB 1 ULPI Clock Control
#define REG_SLCR_APER_CLK_CTRL                           (*(volatile unsigned long*)(SLCR_BASE_ADDR + 0x012Cu))    // AMBA peripheral clock control
#define REG_SLCR_GEM0_RCLK_CTRL                          (*(volatile unsigned long*)(SLCR_BASE_ADDR + 0x0138u))    // GigE 0 Rx Clock and Rx Signals Select
#define REG_SLCR_GEM1_CLK_CTRL                           (*(volatile unsigned long*)(SLCR_BASE_ADDR + 0x013Cu))    // GbE controller 1 clock control
#define REG_SLCR_GEM0_CLK_CTRL                           (*(volatile unsigned long*)(SLCR_BASE_ADDR + 0x0140u))    // GigE 0 Ref Clock Control
#define REG_SLCR_SMC_CLK_CTRL                            (*(volatile unsigned long*)(SLCR_BASE_ADDR + 0x0148u))    // SMC clock control
#define REG_SLCR_LQSPI_CLK_CTRL                          (*(volatile unsigned long*)(SLCR_BASE_ADDR + 0x014Cu))    // QSPI clock control
#define REG_SLCR_SDIO_CLK_CTRL                           (*(volatile unsigned long*)(SLCR_BASE_ADDR + 0x0150u))    // SDIO clock control
#define REG_SLCR_UART_CLK_CTRL                           (*(volatile unsigned long*)(SLCR_BASE_ADDR + 0x0154u))    // UART clock control
#define REG_SLCR_SPI_CLK_CTRL                            (*(volatile unsigned long*)(SLCR_BASE_ADDR + 0x0158u))    // SPI1 clock control
#define REG_SLCR_CAN_CLK_CTRL                            (*(volatile unsigned long*)(SLCR_BASE_ADDR + 0x015Cu))    // CAN clock control
#define REG_SLCR_DBG_CLK_CTRL                            (*(volatile unsigned long*)(SLCR_BASE_ADDR + 0x0164u))    // Debug clock control
#define REG_SLCR_PCAP_CLK_CTRL                           (*(volatile unsigned long*)(SLCR_BASE_ADDR + 0x0168u))    // PCAP clock control
#define REG_SLCR_FPGA0_CLK_CTRL                          (*(volatile unsigned long*)(SLCR_BASE_ADDR + 0x0170u))    // PL Clock 0 Output control
#define REG_SLCR_FPGA1_CLK_CTRL                          (*(volatile unsigned long*)(SLCR_BASE_ADDR + 0x0180u))    // PL Clock 1 Output control
#define REG_SLCR_FPGA2_CLK_CTRL                          (*(volatile unsigned long*)(SLCR_BASE_ADDR + 0x0190u))    // PL Clock 2 output control
#define REG_SLCR_FPGA3_CLK_CTRL                          (*(volatile unsigned long*)(SLCR_BASE_ADDR + 0x01A0u))    // PL Clock 3 output control
#define REG_SLCR_CLK_621_TRUE                            (*(volatile unsigned long*)(SLCR_BASE_ADDR + 0x01C4u))    // CPU Clock Ratio Mode select
#define REG_PSS_RST_CTRL                                 (*(volatile unsigned long*)(SLCR_BASE_ADDR + 0x0200u))    // PS software reset control
#define REG_SLCR_DMAC_RST_CTRL                           (*(volatile unsigned long*)(SLCR_BASE_ADDR + 0x020Cu))    // DMAC Software Reset Control
#define REG_SLCR_USB_RST_CTRL                            (*(volatile unsigned long*)(SLCR_BASE_ADDR + 0x0210u))    // USB Software Reset Control
#define REG_FPGA_RST_CTRL                                (*(volatile unsigned long*)(SLCR_BASE_ADDR + 0x0240u))    // FPGA software reset control
#define REG_SLCR_WDT_CLK_SEL                             (*(volatile unsigned long*)(SLCR_BASE_ADDR + 0x0304u))    // System watchdog clock select
#define REG_SLCR_TZ_DMA_NS                               (*(volatile unsigned long*)(SLCR_BASE_ADDR + 0x0440u))    // DMAC TrustZone Config
#define REG_SLCR_TZ_DMA_IRQ_NS                           (*(volatile unsigned long*)(SLCR_BASE_ADDR + 0x0444u))    // DMAC TrustZone Config for Interrupts
#define REG_SLCR_TZ_DMA_PERIPH_NS                        (*(volatile unsigned long*)(SLCR_BASE_ADDR + 0x0448u))    // DMAC TrustZone Config for Peripherals
#define REG_SLCR_DDR_URGENT                              (*(volatile unsigned long*)(SLCR_BASE_ADDR + 0x0600u))    // DDR Urgent control
#define REG_SLCR_DDR_URGENT_SEL                          (*(volatile unsigned long*)(SLCR_BASE_ADDR + 0x061Cu))    // DDR Urgent select
#define REG_SLCR_MIO_PIN_00                              (*(volatile unsigned long*)(SLCR_BASE_ADDR + 0x0700u))    // MIO Pin 0 Control
#define REG_SLCR_MIO_PIN_01                              (*(volatile unsigned long*)(SLCR_BASE_ADDR + 0x0704u))    // MIO Pin 1 Control
#define REG_SLCR_MIO_PIN_02                              (*(volatile unsigned long*)(SLCR_BASE_ADDR + 0x0708u))    // MIO Pin 2 Control
#define REG_SLCR_MIO_PIN_03                              (*(volatile unsigned long*)(SLCR_BASE_ADDR + 0x070Cu))    // MIO Pin 3 Control
#define REG_SLCR_MIO_PIN_04                              (*(volatile unsigned long*)(SLCR_BASE_ADDR + 0x0710u))    // MIO Pin 4 Control
#define REG_SLCR_MIO_PIN_05                              (*(volatile unsigned long*)(SLCR_BASE_ADDR + 0x0714u))    // MIO Pin 5 Control
#define REG_SLCR_MIO_PIN_06                              (*(volatile unsigned long*)(SLCR_BASE_ADDR + 0x0718u))    // MIO Pin 6 Control
#define REG_SLCR_MIO_PIN_07                              (*(volatile unsigned long*)(SLCR_BASE_ADDR + 0x071Cu))    // MIO Pin 7 Control
#define REG_SLCR_MIO_PIN_08                              (*(volatile unsigned long*)(SLCR_BASE_ADDR + 0x0720u))    // MIO Pin 8 Control
#define REG_SLCR_MIO_PIN_09                              (*(volatile unsigned long*)(SLCR_BASE_ADDR + 0x0724u))    // MIO Pin 9 Control
#define REG_SLCR_MIO_PIN_10                              (*(volatile unsigned long*)(SLCR_BASE_ADDR + 0x0728u))    // MIO Pin 10 Control
#define REG_SLCR_MIO_PIN_11                              (*(volatile unsigned long*)(SLCR_BASE_ADDR + 0x072Cu))    // MIO Pin 11 Control
#define REG_SLCR_MIO_PIN_12                              (*(volatile unsigned long*)(SLCR_BASE_ADDR + 0x0730u))    // MIO Pin 12 Control
#define REG_SLCR_MIO_PIN_13                              (*(volatile unsigned long*)(SLCR_BASE_ADDR + 0x0734u))    // MIO Pin 13 Control
#define REG_SLCR_MIO_PIN_14                              (*(volatile unsigned long*)(SLCR_BASE_ADDR + 0x0738u))    // MIO Pin 14 Control
#define REG_SLCR_MIO_PIN_15                              (*(volatile unsigned long*)(SLCR_BASE_ADDR + 0x073Cu))    // MIO Pin 15 Control
#define REG_SLCR_MIO_PIN_16                              (*(volatile unsigned long*)(SLCR_BASE_ADDR + 0x0740u))    // MIO Pin 16 Control
#define REG_SLCR_MIO_PIN_17                              (*(volatile unsigned long*)(SLCR_BASE_ADDR + 0x0744u))    // MIO Pin 17 Control
#define REG_SLCR_MIO_PIN_18                              (*(volatile unsigned long*)(SLCR_BASE_ADDR + 0x0748u))    // MIO Pin 18 Control
#define REG_SLCR_MIO_PIN_19                              (*(volatile unsigned long*)(SLCR_BASE_ADDR + 0x074Cu))    // MIO Pin 19 Control
#define REG_SLCR_MIO_PIN_20                              (*(volatile unsigned long*)(SLCR_BASE_ADDR + 0x0750u))    // MIO Pin 20 Control
#define REG_SLCR_MIO_PIN_21                              (*(volatile unsigned long*)(SLCR_BASE_ADDR + 0x0754u))    // MIO Pin 21 Control
#define REG_SLCR_MIO_PIN_22                              (*(volatile unsigned long*)(SLCR_BASE_ADDR + 0x0758u))    // MIO Pin 22 Control
#define REG_SLCR_MIO_PIN_23                              (*(volatile unsigned long*)(SLCR_BASE_ADDR + 0x075Cu))    // MIO Pin 23 Control
#define REG_SLCR_MIO_PIN_24                              (*(volatile unsigned long*)(SLCR_BASE_ADDR + 0x0760u))    // MIO Pin 24 Control
#define REG_SLCR_MIO_PIN_25                              (*(volatile unsigned long*)(SLCR_BASE_ADDR + 0x0764u))    // MIO Pin 25 Control
#define REG_SLCR_MIO_PIN_26                              (*(volatile unsigned long*)(SLCR_BASE_ADDR + 0x0768u))    // MIO Pin 26 Control
#define REG_SLCR_MIO_PIN_27                              (*(volatile unsigned long*)(SLCR_BASE_ADDR + 0x076Cu))    // MIO Pin 27 Control
#define REG_SLCR_MIO_PIN_28                              (*(volatile unsigned long*)(SLCR_BASE_ADDR + 0x0770u))    // MIO Pin 28 Control
#define REG_SLCR_MIO_PIN_29                              (*(volatile unsigned long*)(SLCR_BASE_ADDR + 0x0774u))    // MIO Pin 29 Control
#define REG_SLCR_MIO_PIN_30                              (*(volatile unsigned long*)(SLCR_BASE_ADDR + 0x0778u))    // MIO Pin 30 Control
#define REG_SLCR_MIO_PIN_31                              (*(volatile unsigned long*)(SLCR_BASE_ADDR + 0x077Cu))    // MIO Pin 31 Control
#define REG_SLCR_MIO_PIN_32                              (*(volatile unsigned long*)(SLCR_BASE_ADDR + 0x0780u))    // MIO Pin 32 Control
#define REG_SLCR_MIO_PIN_33                              (*(volatile unsigned long*)(SLCR_BASE_ADDR + 0x0784u))    // MIO Pin 33 Control
#define REG_SLCR_MIO_PIN_34                              (*(volatile unsigned long*)(SLCR_BASE_ADDR + 0x0788u))    // MIO Pin 34 Control
#define REG_SLCR_MIO_PIN_35                              (*(volatile unsigned long*)(SLCR_BASE_ADDR + 0x078Cu))    // MIO Pin 35 Control
#define REG_SLCR_MIO_PIN_36                              (*(volatile unsigned long*)(SLCR_BASE_ADDR + 0x0790u))    // MIO Pin 36 Control
#define REG_SLCR_MIO_PIN_37                              (*(volatile unsigned long*)(SLCR_BASE_ADDR + 0x0794u))    // MIO Pin 37 Control
#define REG_SLCR_MIO_PIN_38                              (*(volatile unsigned long*)(SLCR_BASE_ADDR + 0x0798u))    // MIO Pin 38 Control
#define REG_SLCR_MIO_PIN_39                              (*(volatile unsigned long*)(SLCR_BASE_ADDR + 0x079Cu))    // MIO Pin 39 Control
#define REG_SLCR_MIO_PIN_40                              (*(volatile unsigned long*)(SLCR_BASE_ADDR + 0x07A0u))    // MIO Pin 40 Control
#define REG_SLCR_MIO_PIN_41                              (*(volatile unsigned long*)(SLCR_BASE_ADDR + 0x07A4u))    // MIO Pin 41 Control
#define REG_SLCR_MIO_PIN_42                              (*(volatile unsigned long*)(SLCR_BASE_ADDR + 0x07A8u))    // MIO Pin 42 Control
#define REG_SLCR_MIO_PIN_43                              (*(volatile unsigned long*)(SLCR_BASE_ADDR + 0x07ACu))    // MIO Pin 43 Control
#define REG_SLCR_MIO_PIN_44                              (*(volatile unsigned long*)(SLCR_BASE_ADDR + 0x07B0u))    // MIO Pin 44 Control
#define REG_SLCR_MIO_PIN_45                              (*(volatile unsigned long*)(SLCR_BASE_ADDR + 0x07B4u))    // MIO Pin 45 Control
#define REG_SLCR_MIO_PIN_46                              (*(volatile unsigned long*)(SLCR_BASE_ADDR + 0x07B8u))    // MIO Pin 46 Control
#define REG_SLCR_MIO_PIN_47                              (*(volatile unsigned long*)(SLCR_BASE_ADDR + 0x07BCu))    // MIO Pin 47 Control
#define REG_SLCR_MIO_PIN_48                              (*(volatile unsigned long*)(SLCR_BASE_ADDR + 0x07C0u))    // MIO Pin 48 Control
#define REG_SLCR_MIO_PIN_49                              (*(volatile unsigned long*)(SLCR_BASE_ADDR + 0x07C4u))    // MIO Pin 49 Control
#define REG_SLCR_MIO_PIN_50                              (*(volatile unsigned long*)(SLCR_BASE_ADDR + 0x07C8u))    // MIO Pin 50 Control
#define REG_SLCR_MIO_PIN_51                              (*(volatile unsigned long*)(SLCR_BASE_ADDR + 0x07CCu))    // MIO Pin 51 Control
#define REG_SLCR_MIO_PIN_52                              (*(volatile unsigned long*)(SLCR_BASE_ADDR + 0x07D0u))    // MIO Pin 52 Control
#define REG_SLCR_MIO_PIN_53                              (*(volatile unsigned long*)(SLCR_BASE_ADDR + 0x07D4u))    // MIO Pin 53 Control
#define REG_SLCR_LVL_SHFTR_EN                            (*(volatile unsigned long*)(SLCR_BASE_ADDR + 0x0900u))    // Enable for level shifters between PS and PL
#define REG_SLCR_OCM_CFG                                 (*(volatile unsigned long*)(SLCR_BASE_ADDR + 0x0910u))    // OCM Address Mapping
#define REG_SLCR_RESERVED                                (*(volatile unsigned long*)(SLCR_BASE_ADDR + 0x0A1Cu))    // (Reserved); required for L2 Cache config. (see TRM, section 3.4.10 "Programming Model")
#define REG_SLCR_DDRIOB_ADDR0                            (*(volatile unsigned long*)(SLCR_BASE_ADDR + 0x0B40u))
#define REG_SLCR_DDRIOB_ADDR1                            (*(volatile unsigned long*)(SLCR_BASE_ADDR + 0x0B44u))    // DDR IOB Config for BA[2:0], ODT, CS_B, WE_B, RAS_B and CAS_B
#define REG_SLCR_DDRIOB_DATA0                            (*(volatile unsigned long*)(SLCR_BASE_ADDR + 0x0B48u))    // DDR IOB Config for Data 15:0
#define REG_SLCR_DDRIOB_DATA1                            (*(volatile unsigned long*)(SLCR_BASE_ADDR + 0x0B4Cu))    // DDR IOB Config for Data 31:16
#define REG_SLCR_DDRIOB_DIFF0                            (*(volatile unsigned long*)(SLCR_BASE_ADDR + 0x0B50u))    // DDR IOB Config for DQS 1:0
#define REG_SLCR_DDRIOB_DIFF1                            (*(volatile unsigned long*)(SLCR_BASE_ADDR + 0x0B54u))    // DDR IOB Config for DQS 3:2
#define REG_SLCR_DDRIOB_CLOCK                            (*(volatile unsigned long*)(SLCR_BASE_ADDR + 0x0B58u))    // DDR IOB Config for Clock Output
#define REG_SLCR_DDRIOB_DRIVE_SLEW_ADDR                  (*(volatile unsigned long*)(SLCR_BASE_ADDR + 0x0B5Cu))    // Drive and Slew controls for Address and Command pins of the DDR Interface
#define REG_SLCR_DDRIOB_DRIVE_SLEW_DATA                  (*(volatile unsigned long*)(SLCR_BASE_ADDR + 0x0B60u))    // Drive and Slew controls for DQ  pins of the DDR Interface
#define REG_SLCR_DDRIOB_DRIVE_SLEW_DIFF                  (*(volatile unsigned long*)(SLCR_BASE_ADDR + 0x0B64u))    // Drive and Slew controls for DQS  pins of the DDR Interface
#define REG_SLCR_DDRIOB_DRIVE_SLEW_CLOCK                 (*(volatile unsigned long*)(SLCR_BASE_ADDR + 0x0B68u))    // Drive and Slew controls for Clock pins of the DDR Interface
#define REG_SLCR_DDRIOB_DDR_CTRL                         (*(volatile unsigned long*)(SLCR_BASE_ADDR + 0x0B6Cu))    // DDR IOB Buffer Control
#define REG_SLCR_DDRIOB_DCI_CTRL                         (*(volatile unsigned long*)(SLCR_BASE_ADDR + 0x0B70u))    // DDR IOB DCI Config
#define REG_SLCR_DDRIOB_DCI_STATUS                       (*(volatile unsigned long*)(SLCR_BASE_ADDR + 0x0B74u))    // DDR IO Buffer DCI Status

/****** AXI High Priority Port 0 ************************************/
#define AFI_HP0_BASE_ADDR                                (0xF8008000u)
#define REG_HP0_WRCHAN_CTRL                              (*(volatile unsigned long*)(AFI_HP0_BASE_ADDR + 0x0014))  // AXI HP0 write channel control
#define REG_HP0_WRCHAN_ISSCAP                            (*(volatile unsigned long*)(AFI_HP0_BASE_ADDR + 0x0018))  // AXI HP0 write channel issuing capability
#define REG_HP0_WRQOS                                    (*(volatile unsigned long*)(AFI_HP0_BASE_ADDR + 0x001C))  // AXI HP0 write channel Quality of Service
#define REG_HP0_WRDATAFIFO_LVL                           (*(volatile unsigned long*)(AFI_HP0_BASE_ADDR + 0x0020))  // AXI HP0 write data FIFO level

/****** Device Configuration Port ***********************************/
#define DEVC_BASE_ADDR                                   (0xF8007000u)
#define REG_DEVC_CTRL                                    (*(volatile unsigned long*)(DEVC_BASE_ADDR + 0x0000u))    // Device configuration control
#define REG_DEVC_CFG                                     (*(volatile unsigned long*)(DEVC_BASE_ADDR + 0x0008u))    // Device configuration configuration
#define REG_DEVC_INT_STS                                 (*(volatile unsigned long*)(DEVC_BASE_ADDR + 0x000Cu))    // Device configuration interrupt status
#define REG_DEVC_STATUS                                  (*(volatile unsigned long*)(DEVC_BASE_ADDR + 0x0014u))    // Device configuration status
#define REG_DEVC_DMA_SRC_ADDR                            (*(volatile unsigned long*)(DEVC_BASE_ADDR + 0x0018u))    // Device configuration DMA source address
#define REG_DEVC_DMA_DST_ADDR                            (*(volatile unsigned long*)(DEVC_BASE_ADDR + 0x001Cu))    // Device configuration DMA destination address
#define REG_DEVC_DMA_SRC_LEN                             (*(volatile unsigned long*)(DEVC_BASE_ADDR + 0x0020u))    // Device configuration DMA source length (words)
#define REG_DEVC_DMA_DST_LEN                             (*(volatile unsigned long*)(DEVC_BASE_ADDR + 0x0024u))    // Device configuration DMA destination length (words)
#define REG_DEVC_MCTRL                                   (*(volatile unsigned long*)(DEVC_BASE_ADDR + 0x0080u))    // Device configuration miscellaneous control
#define REG_XADCIF_CFG                                   (*(volatile unsigned long*)(DEVC_BASE_ADDR + 0x0100u))    // XADC interface configuration
#define REG_XADCIF_INT_STS                               (*(volatile unsigned long*)(DEVC_BASE_ADDR + 0x0104u))    // XADC interface interrupt status
#define REG_XADCIF_INT_MASK                              (*(volatile unsigned long*)(DEVC_BASE_ADDR + 0x0108u))    // XADC interface interrupt mask
#define REG_XADCIF_INT_MSTS                              (*(volatile unsigned long*)(DEVC_BASE_ADDR + 0x010Cu))    // XADC interface miscellaneous status
#define REG_XADCIF_CMDFIFO                               (*(volatile unsigned long*)(DEVC_BASE_ADDR + 0x0110u))    // XADC interface command FIFO
#define REG_XADCIF_RDFIFO                                (*(volatile unsigned long*)(DEVC_BASE_ADDR + 0x0114u))    // XADC interface data FIFO
#define REG_XADCIF_MCTL                                  (*(volatile unsigned long*)(DEVC_BASE_ADDR + 0x0118u))    // XADC interface miscellaneous control

/****** Triple Timer Counter 0 **************************************/
#define TTC0_BASE_ADDR                                   (0xF8001000u)
#define REG_TTC0_CLOCK_CTRL1                             (*(volatile unsigned long*)(TTC0_BASE_ADDR + 0x0000))
#define REG_TTC0_CLOCK_CTRL2                             (*(volatile unsigned long*)(TTC0_BASE_ADDR + 0x0004))
#define REG_TTC0_CLOCK_CTRL3                             (*(volatile unsigned long*)(TTC0_BASE_ADDR + 0x0008))
#define REG_TTC0_COUNTER_CTRL1                           (*(volatile unsigned long*)(TTC0_BASE_ADDR + 0x000C))
#define REG_TTC0_COUNTER_CTRL2                           (*(volatile unsigned long*)(TTC0_BASE_ADDR + 0x0010))
#define REG_TTC0_COUNTER_CTRL3                           (*(volatile unsigned long*)(TTC0_BASE_ADDR + 0x0014))
#define REG_TTC0_COUNTER_VAL1                            (*(volatile unsigned long*)(TTC0_BASE_ADDR + 0x0018))
#define REG_TTC0_COUNTER_VAL2                            (*(volatile unsigned long*)(TTC0_BASE_ADDR + 0x001C))
#define REG_TTC0_COUNTER_VAL3                            (*(volatile unsigned long*)(TTC0_BASE_ADDR + 0x0020))
#define REG_TTC0_INTERV_CNTR1                            (*(volatile unsigned long*)(TTC0_BASE_ADDR + 0x0024))
#define REG_TTC0_INTERV_CNTR2                            (*(volatile unsigned long*)(TTC0_BASE_ADDR + 0x0028))
#define REG_TTC0_INTERV_CNTR3                            (*(volatile unsigned long*)(TTC0_BASE_ADDR + 0x002C))
#define REG_TTC0_MATCH1_CNTR1                            (*(volatile unsigned long*)(TTC0_BASE_ADDR + 0x0030))
#define REG_TTC0_MATCH1_CNTR2                            (*(volatile unsigned long*)(TTC0_BASE_ADDR + 0x0034))
#define REG_TTC0_MATCH1_CNTR3                            (*(volatile unsigned long*)(TTC0_BASE_ADDR + 0x0038))
#define REG_TTC0_ISR1                                    (*(volatile unsigned long*)(TTC0_BASE_ADDR + 0x0054))
#define REG_TTC0_ISR2                                    (*(volatile unsigned long*)(TTC0_BASE_ADDR + 0x0058))
#define REG_TTC0_ISR3                                    (*(volatile unsigned long*)(TTC0_BASE_ADDR + 0x005C))
#define REG_TTC0_IER1                                    (*(volatile unsigned long*)(TTC0_BASE_ADDR + 0x0060))
#define REG_TTC0_IER2                                    (*(volatile unsigned long*)(TTC0_BASE_ADDR + 0x0064))
#define REG_TTC0_IER3                                    (*(volatile unsigned long*)(TTC0_BASE_ADDR + 0x0068))

/****** System Watchdog Timer ***************************************/
#define SWDT_BASE_ADDR                                   (0xF8005000u)

#define SWDT_RESTART_VALUE                               (0x1999u)

#define REG_SWDT_MODE                                    (*(volatile unsigned long*)(SWDT_BASE_ADDR + 0x0000u))
#define REG_SWDT_CONTROL                                 (*(volatile unsigned long*)(SWDT_BASE_ADDR + 0x0004u))
#define REG_SWDT_RESTART                                 (*(volatile unsigned long*)(SWDT_BASE_ADDR + 0x0008u))
#define REG_SWDT_STATUS                                  (*(volatile unsigned long*)(SWDT_BASE_ADDR + 0x000Cu))

/****** XADC (Using AXI4-Lite Interface) ****************************/
#define XADC_BASE_ADDR                                   (0x43C10000u)                                             // Defined in Vivado
#define REG_XADC_WR_SRR                                  (*(volatile unsigned long*)(XADC_BASE_ADDR + 0x0000u))    // XADC software reset
#define REG_XADC_RD_SR                                   (*(volatile unsigned long*)(XADC_BASE_ADDR + 0x0004u))    // XADC status register
#define REG_XADC_WR_CONVSTR                              (*(volatile unsigned long*)(XADC_BASE_ADDR + 0x000Cu))    // XADC conversion start
#define REG_XADC_WR_SYSMONRR                             (*(volatile unsigned long*)(XADC_BASE_ADDR + 0x0010u))    // XADC hard macro reset
#define REG_XADC_RW_GIER                                 (*(volatile unsigned long*)(XADC_BASE_ADDR + 0x005Cu))    // XADC global interrupt enable
#define REG_XADC_RW_IPISR                                (*(volatile unsigned long*)(XADC_BASE_ADDR + 0x0060u))    // XADC interrupt status
#define REG_XADC_RW_IPIER                                (*(volatile unsigned long*)(XADC_BASE_ADDR + 0x0068u))    // XADC interrupt enable
#define REG_XADC_RD_TEMP                                 (*(volatile unsigned long*)(XADC_BASE_ADDR + 0x0200u))    // XADC status: Temperature
#define REG_XADC_RD_VCCINT                               (*(volatile unsigned long*)(XADC_BASE_ADDR + 0x0204u))    // XADC status: VCCINT
#define REG_XADC_RD_VCCAUX                               (*(volatile unsigned long*)(XADC_BASE_ADDR + 0x0208u))    // XADC status: VCCAUX
#define REG_XADC_RD_VCCPINT                              (*(volatile unsigned long*)(XADC_BASE_ADDR + 0x0234u))    // XADC status: VCCPINT
#define REG_XADC_RD_VCCPAUX                              (*(volatile unsigned long*)(XADC_BASE_ADDR + 0x0238u))    // XADC status: VCCPAUX
#define REG_XADC_RD_VCCDDR                               (*(volatile unsigned long*)(XADC_BASE_ADDR + 0x023Cu))    // XADC status: VCCDDR
#define REG_XADC_RD_VDIFF0                               (*(volatile unsigned long*)(XADC_BASE_ADDR + 0x0248u))    // XADC status: VDIFF0
#define REG_XADC_RD_VDIFF1                               (*(volatile unsigned long*)(XADC_BASE_ADDR + 0x024Cu))    // XADC status: VDIFF1
#define REG_XADC_RD_VDIFF2                               (*(volatile unsigned long*)(XADC_BASE_ADDR + 0x026Cu))    // XADC status: VDIFF2
#define REG_XADC_RD_VTREF                                (*(volatile unsigned long*)(XADC_BASE_ADDR + 0x0270u))    // XADC status: VTREF
#define REG_XADC_RD_VTSUPPLY                             (*(volatile unsigned long*)(XADC_BASE_ADDR + 0x0254u))    // XADC status: VTSUPPLY
#define REG_XADC_RD_VBUS                                 (*(volatile unsigned long*)(XADC_BASE_ADDR + 0x0274u))    // XADC status: VBUS
#define REG_XADC_RD_TEMP_MAX                             (*(volatile unsigned long*)(XADC_BASE_ADDR + 0x0280u))    // XADC status: Max. temperature
#define REG_XADC_RD_VCCINT_MAX                           (*(volatile unsigned long*)(XADC_BASE_ADDR + 0x0284u))    // XADC status: Max. VCCINT
#define REG_XADC_RD_VCCAUX_MAX                           (*(volatile unsigned long*)(XADC_BASE_ADDR + 0x0288u))    // XADC status: Max. VCCAUX
#define REG_XADC_RD_TEMP_MIN                             (*(volatile unsigned long*)(XADC_BASE_ADDR + 0x0290u))    // XADC status: Min. temperature
#define REG_XADC_RD_VCCINT_MIN                           (*(volatile unsigned long*)(XADC_BASE_ADDR + 0x0294u))    // XADC status: Min. VCCINT
#define REG_XADC_RD_VCCAUX_MIN                           (*(volatile unsigned long*)(XADC_BASE_ADDR + 0x0298u))    // XADC status: Min. VCCAUX
#define REG_XADC_RD_VCCPINT_MAX                          (*(volatile unsigned long*)(XADC_BASE_ADDR + 0x02A0u))    // XADC status: Max. VCCPINT
#define REG_XADC_RD_VCCPAUX_MAX                          (*(volatile unsigned long*)(XADC_BASE_ADDR + 0x02A4u))    // XADC status: Max. VCCPAUX
#define REG_XADC_RD_VCCDDR_MAX                           (*(volatile unsigned long*)(XADC_BASE_ADDR + 0x02A8u))    // XADC status: Max. VCCDDR
#define REG_XADC_RD_VCCPINT_MIN                          (*(volatile unsigned long*)(XADC_BASE_ADDR + 0x02B0u))    // XADC status: Min. VCCPINT
#define REG_XADC_RD_VCCPAUX_MIN                          (*(volatile unsigned long*)(XADC_BASE_ADDR + 0x02B4u))    // XADC status: Min. VCCPAUX
#define REG_XADC_RD_VCCDDR_MIN                           (*(volatile unsigned long*)(XADC_BASE_ADDR + 0x02B8u))    // XADC status: Min. VCCDDR
#define REG_XADC_WR_CFG0                                 (*(volatile unsigned long*)(XADC_BASE_ADDR + 0x0300u))    // XADC configuration register 0
#define REG_XADC_WR_CFG1                                 (*(volatile unsigned long*)(XADC_BASE_ADDR + 0x0304u))    // XADC configuration register 1
#define REG_XADC_WR_CFG2                                 (*(volatile unsigned long*)(XADC_BASE_ADDR + 0x0308u))    // XADC configuration register 2
#define REG_XADC_WR_SEQ0                                 (*(volatile unsigned long*)(XADC_BASE_ADDR + 0x0320u))    // XADC sequence register 0
#define REG_XADC_WR_SEQ1                                 (*(volatile unsigned long*)(XADC_BASE_ADDR + 0x0324u))    // XADC sequence register 1
#define REG_XADC_WR_SEQ2                                 (*(volatile unsigned long*)(XADC_BASE_ADDR + 0x0328u))    // XADC sequence register 2
#define REG_XADC_WR_SEQ3                                 (*(volatile unsigned long*)(XADC_BASE_ADDR + 0x032Cu))    // XADC sequence register 3
#define REG_XADC_WR_SEQ4                                 (*(volatile unsigned long*)(XADC_BASE_ADDR + 0x0330u))    // XADC sequence register 4
#define REG_XADC_WR_SEQ5                                 (*(volatile unsigned long*)(XADC_BASE_ADDR + 0x0334u))    // XADC sequence register 5
#define REG_XADC_WR_SEQ6                                 (*(volatile unsigned long*)(XADC_BASE_ADDR + 0x0338u))    // XADC sequence register 6
#define REG_XADC_WR_SEQ7                                 (*(volatile unsigned long*)(XADC_BASE_ADDR + 0x033Cu))    // XADC sequence register 7

/****** QSPI controller *********************************************/
#define QSPIC_BASE_ADDR                                  (0xE000D000u)
#define REG_QSPIC_CONFIG_REG                             (*(volatile unsigned long*)(QSPIC_BASE_ADDR + 0x0000u))   // QSPI configuration register
#define REG_QSPIC_INTR_STATUS_REG                        (*(volatile unsigned long*)(QSPIC_BASE_ADDR + 0x0004u))   // QSPI interrupt status register
#define REG_QSPIC_INTRPT_EN_REG                          (*(volatile unsigned long*)(QSPIC_BASE_ADDR + 0x0008u))   // Interrupt Enable register.
#define REG_QSPIC_INTRPT_DIS_REG                         (*(volatile unsigned long*)(QSPIC_BASE_ADDR + 0x000Cu))   // Interrupt disable register.
#define REG_QSPIC_INTRPT_MASK_REG                        (*(volatile unsigned long*)(QSPIC_BASE_ADDR + 0x0010u))   // Interrupt mask register
#define REG_QSPIC_EN_REG                                 (*(volatile unsigned long*)(QSPIC_BASE_ADDR + 0x0014u))   // SPI_Enable Register
#define REG_QSPIC_DELAY_REG                              (*(volatile unsigned long*)(QSPIC_BASE_ADDR + 0x0018u))   // Delay Register
#define REG_QSPIC_TXD0                                   (*(volatile unsigned long*)(QSPIC_BASE_ADDR + 0x001Cu))   // Transmit Data Register. Keyhole addresses for the Transmit data FIFO. See also TXD1-3.
#define REG_QSPIC_RX_DATA_REG                            (*(volatile unsigned long*)(QSPIC_BASE_ADDR + 0x0020u))   // Receive Data Register
#define REG_QSPIC_SLAVE_IDLE_COUNT_REG                   (*(volatile unsigned long*)(QSPIC_BASE_ADDR + 0x0024u))   // Slave Idle Count Register
#define REG_QSPIC_TX_THRES_REG                           (*(volatile unsigned long*)(QSPIC_BASE_ADDR + 0x0028u))   // TX_FIFO Threshold Register
#define REG_QSPIC_RX_THRES_REG                           (*(volatile unsigned long*)(QSPIC_BASE_ADDR + 0x002Cu))   // RX FIFO Threshold Register
#define REG_QSPIC_GPIO                                   (*(volatile unsigned long*)(QSPIC_BASE_ADDR + 0x0030u))   // General Purpose Inputs and Outputs Register for the Quad-SPI Controller core
#define REG_QSPIC_LPBK_DLY_ADJ                           (*(volatile unsigned long*)(QSPIC_BASE_ADDR + 0x0038u))   // Loopback Master Clock Delay Adjustment Register
#define REG_QSPIC_TXD1                                   (*(volatile unsigned long*)(QSPIC_BASE_ADDR + 0x0080u))   // Transmit Data Register. Keyhole addresses for the Transmit data FIFO.
#define REG_QSPIC_TXD2                                   (*(volatile unsigned long*)(QSPIC_BASE_ADDR + 0x0084u))   // Transmit Data Register. Keyhole addresses for the Transmit data FIFO.
#define REG_QSPIC_TXD3                                   (*(volatile unsigned long*)(QSPIC_BASE_ADDR + 0x0088u))   // Transmit Data Register. Keyhole addresses for the Transmit data FIFO.
#define REG_QSPIC_LQSPI_CFG                              (*(volatile unsigned long*)(QSPIC_BASE_ADDR + 0x00A0u))   // Configuration Register specifically for the Linear Quad-SPI Controller
#define REG_QSPIC_LQSPI_STS                              (*(volatile unsigned long*)(QSPIC_BASE_ADDR + 0x00A4u))   // Status Register specifically for the Linear Quad-SPI Controller
#define REG_QSPIC_MOD_ID                                 (*(volatile unsigned long*)(QSPIC_BASE_ADDR + 0x00FCu))   // Module Identification register

/****** MPCORE ******************************************************/
#define MPCORE_BASE_ADDR                                 (0xF8F00000u)
#define MPCORE_SCU_CONTROL                               (*(volatile unsigned long*)(MPCORE_BASE_ADDR + 0x0000u))
#define MPCORE_ICDICFR2                                  (*(volatile unsigned long*)(MPCORE_BASE_ADDR + 0x1C08u))  // Interrupt sensitivity control IRQ ID32-47
#define MPCORE_SCU_CONTROL                               (*(volatile unsigned long*)(MPCORE_BASE_ADDR + 0x0000u))
#define MPCORE_WATCHDOG_LOAD_REG                         (*(volatile unsigned long*)(MPCORE_BASE_ADDR + 0x0620u))
#define MPCORE_WATCHDOG_COUNTER_REG                      (*(volatile unsigned long*)(MPCORE_BASE_ADDR + 0x0624u))
#define MPCORE_WATCHDOG_CONTROL_REG                      (*(volatile unsigned long*)(MPCORE_BASE_ADDR + 0x0628u))
#define MPCORE_WATCHDOG_INTERRUPT_STATUS_REG             (*(volatile unsigned long*)(MPCORE_BASE_ADDR + 0x062Cu))
#define MPCORE_WATCHDOG_RESET_STATUS_REG                 (*(volatile unsigned long*)(MPCORE_BASE_ADDR + 0x0630u))
#define MPCORE_WATCHDOG_DISABLE_REG                      (*(volatile unsigned long*)(MPCORE_BASE_ADDR + 0x0634u))

/****** L2 Cache Controller *****************************************/
#define L2CACHE_BASE_ADDR                                (0xF8F02000u)
#define L2C_REG1_CONTROL                                 (*(volatile unsigned long*)(L2CACHE_BASE_ADDR + 0x0100u))
#define L2C_REG1_AUX_CONTROL                             (*(volatile unsigned long*)(L2CACHE_BASE_ADDR + 0x0104u))
#define L2C_REG1_TAG_RAM_CTRL                            (*(volatile unsigned long*)(L2CACHE_BASE_ADDR + 0x0108u))
#define L2C_REG1_DATA_RAM_CTRL                           (*(volatile unsigned long*)(L2CACHE_BASE_ADDR + 0x010Cu))
#define L2C_REG2_INT_RAW_STATUS                          (*(volatile unsigned long*)(L2CACHE_BASE_ADDR + 0x021Cu))
#define L2C_REG2_INT_CLEAR                               (*(volatile unsigned long*)(L2CACHE_BASE_ADDR + 0x0220u))
#define L2C_DUMMY_CACHE_SYNC                             (*(volatile unsigned long*)(L2CACHE_BASE_ADDR + 0x0740u))
#define L2C_REG7_INV_PA                                  (*(volatile unsigned long*)(L2CACHE_BASE_ADDR + 0x0770u))
#define L2C_REG7_INV_WAY                                 (*(volatile unsigned long*)(L2CACHE_BASE_ADDR + 0x077Cu))
#define L2C_REG7_CLEAN_PA                                (*(volatile unsigned long*)(L2CACHE_BASE_ADDR + 0x07B0u))
#define L2C_REG7_CLEAN_WAY                               (*(volatile unsigned long*)(L2CACHE_BASE_ADDR + 0x07BCu))
#define L2C_REG7_CLEAN_INV_PA                            (*(volatile unsigned long*)(L2CACHE_BASE_ADDR + 0x07F0u))
#define L2C_REG7_CLEAN_INV_WAY                           (*(volatile unsigned long*)(L2CACHE_BASE_ADDR + 0x07FCu))
#define L2C_REG15_DBG_CTRL                               (*(volatile unsigned long*)(L2CACHE_BASE_ADDR + 0x0F40u))
#define L2C_REG15_POWER_CTRL                             (*(volatile unsigned long*)(L2CACHE_BASE_ADDR + 0x0F80u))

/****** DEVCFG registers ********************************************/
#define DEVCFG_BASE_ADDR                                 (0xF8007000u)
#define REG_DEVCFG_CTRL                                  (*(volatile unsigned long*)(DEVCFG_BASE_ADDR + 0x000u))
#define REG_DEVCFG_LOCK                                  (*(volatile unsigned long*)(DEVCFG_BASE_ADDR + 0x004u))
#define REG_DEVCFG_CFG                                   (*(volatile unsigned long*)(DEVCFG_BASE_ADDR + 0x008u))
#define REG_DEVCFG_INT_STS                               (*(volatile unsigned long*)(DEVCFG_BASE_ADDR + 0x00Cu))
#define REG_DEVCFG_INT_MASK                              (*(volatile unsigned long*)(DEVCFG_BASE_ADDR + 0x010u))
#define REG_DEVCFG_STATUS                                (*(volatile unsigned long*)(DEVCFG_BASE_ADDR + 0x014u))
#define REG_DEVCFG_DMA_SRC_ADDR                          (*(volatile unsigned long*)(DEVCFG_BASE_ADDR + 0x018u))
#define REG_DEVCFG_DMA_DST_ADDR                          (*(volatile unsigned long*)(DEVCFG_BASE_ADDR + 0x01Cu))
#define REG_DEVCFG_DMA_SRC_LEN                           (*(volatile unsigned long*)(DEVCFG_BASE_ADDR + 0x020u))
#define REG_DEVCFG_DMA_DEST_LEN                          (*(volatile unsigned long*)(DEVCFG_BASE_ADDR + 0x024u))
#define REG_DEVCFG_MULTIBOOT_ADDR                        (*(volatile unsigned long*)(DEVCFG_BASE_ADDR + 0x02Cu))
#define REG_DEVCFG_MULTIBOOT_ADDR_BYTE0                  (*(volatile unsigned char*)(DEVCFG_BASE_ADDR + 0x02Cu))
#define REG_DEVCFG_MULTIBOOT_ADDR_BYTE1                  (*(volatile unsigned char*)(DEVCFG_BASE_ADDR + 0x02Du))
#define REG_DEVCFG_MULTIBOOT_ADDR_BYTE2                  (*(volatile unsigned char*)(DEVCFG_BASE_ADDR + 0x02Eu))
#define REG_DEVCFG_MULTIBOOT_ADDR_BYTE3                  (*(volatile unsigned char*)(DEVCFG_BASE_ADDR + 0x02Fu))
#define REG_DEVCFG_UNLOCK                                (*(volatile unsigned long*)(DEVCFG_BASE_ADDR + 0x034u))
#define REG_DEVCFG_MCTRL                                 (*(volatile unsigned long*)(DEVCFG_BASE_ADDR + 0x080u))
#define REG_DEVCFG_XADCIF_CFG                            (*(volatile unsigned long*)(DEVCFG_BASE_ADDR + 0x100u))
#define REG_DEVCFG_XADCIF_INT_STS                        (*(volatile unsigned long*)(DEVCFG_BASE_ADDR + 0x104u))
#define REG_DEVCFG_XADCIF_INT_MASK                       (*(volatile unsigned long*)(DEVCFG_BASE_ADDR + 0x108u))
#define REG_DEVCFG_XADCIF_MSTS                           (*(volatile unsigned long*)(DEVCFG_BASE_ADDR + 0x10Cu))
#define REG_DEVCFG_XADCIF_CMDFIFO                        (*(volatile unsigned long*)(DEVCFG_BASE_ADDR + 0x110u))
#define REG_DEVCFG_XADCIF_RDFIFO                         (*(volatile unsigned long*)(DEVCFG_BASE_ADDR + 0x114u))
#define REG_DEVCFG_XADCIF_MCTL                           (*(volatile unsigned long*)(DEVCFG_BASE_ADDR + 0x118u))

/****** USB registers **********************************************/
#define USB0_BASE_ADDR                                   (0xE0002000)
#define REG_USB0_ID                                      (*(volatile unsigned long*)(USB0_BASE_ADDR + 0x00000000u))  // IP version and revision, read-only
#define REG_USB0_HWGENERAL                               (*(volatile unsigned long*)(USB0_BASE_ADDR + 0x00000004u))  // Misc IP config constants, read-only
#define REG_USB0_HWHOST                                  (*(volatile unsigned long*)(USB0_BASE_ADDR + 0x00000008u))  // Host Mode IP config constants, read-only
#define REG_USB0_HWDEVICE                                (*(volatile unsigned long*)(USB0_BASE_ADDR + 0x0000000Cu))  // Device Mode IP config constants, read-only
#define REG_USB0_HWTXBUF                                 (*(volatile unsigned long*)(USB0_BASE_ADDR + 0x00000010u))  // TxBuffer IP config constants, read-only
#define REG_USB0_HWRXBUF                                 (*(volatile unsigned long*)(USB0_BASE_ADDR + 0x00000014u))  // IP constants, RX buffer constants, read-only
#define REG_USB0_GPTIMER0LD                              (*(volatile unsigned long*)(USB0_BASE_ADDR + 0x00000080u))  // GP Timer 0 Load Value
#define REG_USB0_GPTIMER0CTRL                            (*(volatile unsigned long*)(USB0_BASE_ADDR + 0x00000084u))  // GP Timer 1 Control
#define REG_USB0_GPTIMER1LD                              (*(volatile unsigned long*)(USB0_BASE_ADDR + 0x00000088u))  // GP Timer 1 Load Value
#define REG_USB0_GPTIMER1CTRL                            (*(volatile unsigned long*)(USB0_BASE_ADDR + 0x0000008Cu))  // GP Timer 1 Control
#define REG_USB0_SBUSCFG                                 (*(volatile unsigned long*)(USB0_BASE_ADDR + 0x00000090u))  // DMA Master AHB Burst Mode
#define REG_USB0_CAPLENGTH_HCIVERSION                    (*(volatile unsigned long*)(USB0_BASE_ADDR + 0x00000100u))  // EHCI Addr Space and HCI constants, read-only
#define REG_USB0_HCSPARAMS                               (*(volatile unsigned long*)(USB0_BASE_ADDR + 0x00000104u))  // TT counts and EHCI HCS constants, read-only
#define REG_USB0_HCCPARAMS                               (*(volatile unsigned long*)(USB0_BASE_ADDR + 0x00000108u))  // EHCI Host configuration constants
#define REG_USB0_DCIVERSION                              (*(volatile unsigned long*)(USB0_BASE_ADDR + 0x00000120u))  // Device Mode CI version constant
#define REG_USB0_DCCPARAMS                               (*(volatile unsigned long*)(USB0_BASE_ADDR + 0x00000124u))  // EHCI, device and endpoint capabilities
#define REG_USB0_USBCMD                                  (*(volatile unsigned long*)(USB0_BASE_ADDR + 0x00000140u))  // USB Commands (EHCI extended)
#define REG_USB0_USBSTS                                  (*(volatile unsigned long*)(USB0_BASE_ADDR + 0x00000144u))  // Interrupt/Raw Status (EHCI extended) (Host/Device)
#define REG_USB0_USBINTR                                 (*(volatile unsigned long*)(USB0_BASE_ADDR + 0x00000148u))  // Interrrupts and Enables
#define REG_USB0_FRINDEX                                 (*(volatile unsigned long*)(USB0_BASE_ADDR + 0x0000014Cu))  // Frame List Index
#define REG_USB0_PERIODICLISTBASE_DEVICEADDR             (*(volatile unsigned long*)(USB0_BASE_ADDR + 0x00000154u))  // Host/Device Address dual-use
#define REG_USB0_ASYNCLISTADDR_ENDPOINTLISTADDR          (*(volatile unsigned long*)(USB0_BASE_ADDR + 0x00000158u))  // Host/Device dual-use
#define REG_USB0_TTCTRL                                  (*(volatile unsigned long*)(USB0_BASE_ADDR + 0x0000015Cu))  // TT Control
#define REG_USB0_BURSTSIZE                               (*(volatile unsigned long*)(USB0_BASE_ADDR + 0x00000160u))  // Burst Size
#define REG_USB0_TXFILLTUNING                            (*(volatile unsigned long*)(USB0_BASE_ADDR + 0x00000164u))  // TxFIFO Fill Tuning
#define REG_USB0_TXTTFILLTUNING                          (*(volatile unsigned long*)(USB0_BASE_ADDR + 0x00000168u))  // TT TX latency FIFO
#define REG_USB0_IC_USB                                  (*(volatile unsigned long*)(USB0_BASE_ADDR + 0x0000016Cu))  // Low and Fast Speed Control constants
#define REG_USB0_ULPI_VIEWPORT                           (*(volatile unsigned long*)(USB0_BASE_ADDR + 0x00000170u))  // ULPI Viewport
#define REG_USB0_ENDPTNAK                                (*(volatile unsigned long*)(USB0_BASE_ADDR + 0x00000178u))  // Endpoint NAK (Device mode)
#define REG_USB0_ENDPTNAKEN                              (*(volatile unsigned long*)(USB0_BASE_ADDR + 0x0000017Cu))  // Endpoint NAK (Device mode)
#define REG_USB0_CONFIGFLAG                              (*(volatile unsigned long*)(USB0_BASE_ADDR + 0x00000180u))  // reserved
#define REG_USB0_PORTSC1                                 (*(volatile unsigned long*)(USB0_BASE_ADDR + 0x00000184u))  // Port Status & Control
#define REG_USB0_OTGSC                                   (*(volatile unsigned long*)(USB0_BASE_ADDR + 0x000001A4u))  // OTG Status and Control
#define REG_USB0_USBMODE                                 (*(volatile unsigned long*)(USB0_BASE_ADDR + 0x000001A8u))  // USB Mode Selection
#define REG_USB0_ENDPTSETUPSTAT                          (*(volatile unsigned long*)(USB0_BASE_ADDR + 0x000001ACu))  // Endpoint Status Setup (Device mode)
#define REG_USB0_ENDPTPRIME                              (*(volatile unsigned long*)(USB0_BASE_ADDR + 0x000001B0u))  // Endpoint Primer (Device mode)
#define REG_USB0_ENDPTFLUSH                              (*(volatile unsigned long*)(USB0_BASE_ADDR + 0x000001B4u))  // Endpoint Flush (Device mode)
#define REG_USB0_ENDPTSTAT                               (*(volatile unsigned long*)(USB0_BASE_ADDR + 0x000001B8u))  // Endpoint Buffer Ready Status (Device mode), RO
#define REG_USB0_ENDPTCOMPLETE                           (*(volatile unsigned long*)(USB0_BASE_ADDR + 0x000001BCu))  // Endpoint Tx Complete (Device mode)
#define REG_USB0_ENDPTCTRL0                              (*(volatile unsigned long*)(USB0_BASE_ADDR + 0x000001C0u))  // Endpoint 0 (Device mode)
#define REG_USB0_ENDPTCTRL1                              (*(volatile unsigned long*)(USB0_BASE_ADDR + 0x000001C4u))  // Endpoints 1 to 11 (Device mode)
#define REG_USB0_ENDPTCTRL2                              (*(volatile unsigned long*)(USB0_BASE_ADDR + 0x000001C8u))  // Endpoints 1 to 11 (Device mode)
#define REG_USB0_ENDPTCTRL3                              (*(volatile unsigned long*)(USB0_BASE_ADDR + 0x000001CCu))  // Endpoints 1 to 11 (Device mode)
#define REG_USB0_ENDPTCTRL4                              (*(volatile unsigned long*)(USB0_BASE_ADDR + 0x000001D0u))  // Endpoints 1 to 11 (Device mode)
#define REG_USB0_ENDPTCTRL5                              (*(volatile unsigned long*)(USB0_BASE_ADDR + 0x000001D4u))  // Endpoints 1 to 11 (Device mode)
#define REG_USB0_ENDPTCTRL6                              (*(volatile unsigned long*)(USB0_BASE_ADDR + 0x000001D8u))  // Endpoints 1 to 11 (Device mode)
#define REG_USB0_ENDPTCTRL7                              (*(volatile unsigned long*)(USB0_BASE_ADDR + 0x000001DCu))  // Endpoints 1 to 11 (Device mode)
#define REG_USB0_ENDPTCTRL8                              (*(volatile unsigned long*)(USB0_BASE_ADDR + 0x000001E0u))  // Endpoints 1 to 11 (Device mode)
#define REG_USB0_ENDPTCTRL9                              (*(volatile unsigned long*)(USB0_BASE_ADDR + 0x000001E4u))  // Endpoints 1 to 11 (Device mode)
#define REG_USB0_ENDPTCTRL10                             (*(volatile unsigned long*)(USB0_BASE_ADDR + 0x000001E8u))  // Endpoints 1 to 11 (Device mode)
#define REG_USB0_ENDPTCTRL11                             (*(volatile unsigned long*)(USB0_BASE_ADDR + 0x000001ECu))  // Endpoints 1 to 11 (Device mode)

#define USB1_BASE_ADDR                                   (0xE0003000u)
#define REG_USB1_ID                                      (*(volatile unsigned long*)(USB1_BASE_ADDR + 0x00000000u))  // IP version and revision, read-only
#define REG_USB1_HWGENERAL                               (*(volatile unsigned long*)(USB1_BASE_ADDR + 0x00000004u))  // Misc IP config constants, read-only
#define REG_USB1_HWHOST                                  (*(volatile unsigned long*)(USB1_BASE_ADDR + 0x00000008u))  // Host Mode IP config constants, read-only
#define REG_USB1_HWDEVICE                                (*(volatile unsigned long*)(USB1_BASE_ADDR + 0x0000000Cu))  // Device Mode IP config constants, read-only
#define REG_USB1_HWTXBUF                                 (*(volatile unsigned long*)(USB1_BASE_ADDR + 0x00000010u))  // TxBuffer IP config constants, read-only
#define REG_USB1_HWRXBUF                                 (*(volatile unsigned long*)(USB1_BASE_ADDR + 0x00000014u))  // IP constants, RX buffer constants, read-only
#define REG_USB1_GPTIMER0LD                              (*(volatile unsigned long*)(USB1_BASE_ADDR + 0x00000080u))  // GP Timer 0 Load Value
#define REG_USB1_GPTIMER0CTRL                            (*(volatile unsigned long*)(USB1_BASE_ADDR + 0x00000084u))  // GP Timer 1 Control
#define REG_USB1_GPTIMER1LD                              (*(volatile unsigned long*)(USB1_BASE_ADDR + 0x00000088u))  // GP Timer 1 Load Value
#define REG_USB1_GPTIMER1CTRL                            (*(volatile unsigned long*)(USB1_BASE_ADDR + 0x0000008Cu))  // GP Timer 1 Control
#define REG_USB1_SBUSCFG                                 (*(volatile unsigned long*)(USB1_BASE_ADDR + 0x00000090u))  // DMA Master AHB Burst Mode
#define REG_USB1_CAPLENGTH_HCIVERSION                    (*(volatile unsigned long*)(USB1_BASE_ADDR + 0x00000100u))  // EHCI Addr Space and HCI constants, read-only
#define REG_USB1_HCSPARAMS                               (*(volatile unsigned long*)(USB1_BASE_ADDR + 0x00000104u))  // TT counts and EHCI HCS constants, read-only
#define REG_USB1_HCCPARAMS                               (*(volatile unsigned long*)(USB1_BASE_ADDR + 0x00000108u))  // EHCI Host configuration constants
#define REG_USB1_DCIVERSION                              (*(volatile unsigned long*)(USB1_BASE_ADDR + 0x00000120u))  // Device Mode CI version constant
#define REG_USB1_DCCPARAMS                               (*(volatile unsigned long*)(USB1_BASE_ADDR + 0x00000124u))  // EHCI, device and endpoint capabilities
#define REG_USB1_USBCMD                                  (*(volatile unsigned long*)(USB1_BASE_ADDR + 0x00000140u))  // USB Commands (EHCI extended)
#define REG_USB1_USBSTS                                  (*(volatile unsigned long*)(USB1_BASE_ADDR + 0x00000144u))  // Interrupt/Raw Status (EHCI extended) (Host/Device)
#define REG_USB1_USBINTR                                 (*(volatile unsigned long*)(USB1_BASE_ADDR + 0x00000148u))  // Interrrupts and Enables
#define REG_USB1_FRINDEX                                 (*(volatile unsigned long*)(USB1_BASE_ADDR + 0x0000014Cu))  // Frame List Index
#define REG_USB1_PERIODICLISTBASE_DEVICEADDR             (*(volatile unsigned long*)(USB1_BASE_ADDR + 0x00000154u))  // Host/Device Address dual-use
#define REG_USB1_ASYNCLISTADDR_ENDPOINTLISTADDR          (*(volatile unsigned long*)(USB1_BASE_ADDR + 0x00000158u))  // Host/Device dual-use
#define REG_USB1_TTCTRL                                  (*(volatile unsigned long*)(USB1_BASE_ADDR + 0x0000015Cu))  // TT Control
#define REG_USB1_BURSTSIZE                               (*(volatile unsigned long*)(USB1_BASE_ADDR + 0x00000160u))  // Burst Size
#define REG_USB1_TXFILLTUNING                            (*(volatile unsigned long*)(USB1_BASE_ADDR + 0x00000164u))  // TxFIFO Fill Tuning
#define REG_USB1_TXTTFILLTUNING                          (*(volatile unsigned long*)(USB1_BASE_ADDR + 0x00000168u))  // TT TX latency FIFO
#define REG_USB1_IC_USB                                  (*(volatile unsigned long*)(USB1_BASE_ADDR + 0x0000016Cu))  // Low and Fast Speed Control constants
#define REG_USB1_ULPI_VIEWPORT                           (*(volatile unsigned long*)(USB1_BASE_ADDR + 0x00000170u))  // ULPI Viewport
#define REG_USB1_ENDPTNAK                                (*(volatile unsigned long*)(USB1_BASE_ADDR + 0x00000178u))  // Endpoint NAK (Device mode)
#define REG_USB1_ENDPTNAKEN                              (*(volatile unsigned long*)(USB1_BASE_ADDR + 0x0000017Cu))  // Endpoint NAK (Device mode)
#define REG_USB1_CONFIGFLAG                              (*(volatile unsigned long*)(USB1_BASE_ADDR + 0x00000180u))  // reserved
#define REG_USB1_PORTSC1                                 (*(volatile unsigned long*)(USB1_BASE_ADDR + 0x00000184u))  // Port Status & Control
#define REG_USB1_OTGSC                                   (*(volatile unsigned long*)(USB1_BASE_ADDR + 0x000001A4u))  // OTG Status and Control
#define REG_USB1_USBMODE                                 (*(volatile unsigned long*)(USB1_BASE_ADDR + 0x000001A8u))  // USB Mode Selection
#define REG_USB1_ENDPTSETUPSTAT                          (*(volatile unsigned long*)(USB1_BASE_ADDR + 0x000001ACu))  // Endpoint Status Setup (Device mode)
#define REG_USB1_ENDPTPRIME                              (*(volatile unsigned long*)(USB1_BASE_ADDR + 0x000001B0u))  // Endpoint Primer (Device mode)
#define REG_USB1_ENDPTFLUSH                              (*(volatile unsigned long*)(USB1_BASE_ADDR + 0x000001B4u))  // Endpoint Flush (Device mode)
#define REG_USB1_ENDPTSTAT                               (*(volatile unsigned long*)(USB1_BASE_ADDR + 0x000001B8u))  // Endpoint Buffer Ready Status (Device mode), RO
#define REG_USB1_ENDPTCOMPLETE                           (*(volatile unsigned long*)(USB1_BASE_ADDR + 0x000001BCu))  // Endpoint Tx Complete (Device mode)
#define REG_USB1_ENDPTCTRL0                              (*(volatile unsigned long*)(USB1_BASE_ADDR + 0x000001C0u))  // Endpoint 0 (Device mode)
#define REG_USB1_ENDPTCTRL1                              (*(volatile unsigned long*)(USB1_BASE_ADDR + 0x000001C4u))  // Endpoints 1 to 11 (Device mode)
#define REG_USB1_ENDPTCTRL2                              (*(volatile unsigned long*)(USB1_BASE_ADDR + 0x000001C8u))  // Endpoints 1 to 11 (Device mode)
#define REG_USB1_ENDPTCTRL3                              (*(volatile unsigned long*)(USB1_BASE_ADDR + 0x000001CCu))  // Endpoints 1 to 11 (Device mode)
#define REG_USB1_ENDPTCTRL4                              (*(volatile unsigned long*)(USB1_BASE_ADDR + 0x000001D0u))  // Endpoints 1 to 11 (Device mode)
#define REG_USB1_ENDPTCTRL5                              (*(volatile unsigned long*)(USB1_BASE_ADDR + 0x000001D4u))  // Endpoints 1 to 11 (Device mode)
#define REG_USB1_ENDPTCTRL6                              (*(volatile unsigned long*)(USB1_BASE_ADDR + 0x000001D8u))  // Endpoints 1 to 11 (Device mode)
#define REG_USB1_ENDPTCTRL7                              (*(volatile unsigned long*)(USB1_BASE_ADDR + 0x000001DCu))  // Endpoints 1 to 11 (Device mode)
#define REG_USB1_ENDPTCTRL8                              (*(volatile unsigned long*)(USB1_BASE_ADDR + 0x000001E0u))  // Endpoints 1 to 11 (Device mode)
#define REG_USB1_ENDPTCTRL9                              (*(volatile unsigned long*)(USB1_BASE_ADDR + 0x000001E4u))  // Endpoints 1 to 11 (Device mode)
#define REG_USB1_ENDPTCTRL10                             (*(volatile unsigned long*)(USB1_BASE_ADDR + 0x000001E8u))  // Endpoints 1 to 11 (Device mode)
#define REG_USB1_ENDPTCTRL11                             (*(volatile unsigned long*)(USB1_BASE_ADDR + 0x000001ECu))  // Endpoints 1 to 11 (Device mode)

/****** UART registers **********************************************/
#define UART0_BASE_ADDR                                  (0xE0000000u)
#define UART1_BASE_ADDR                                  (0xE0001000u)

#define REG_UART0_CR                                     (*(volatile unsigned long*)(UART0_BASE_ADDR + 0x00000000u))
#define REG_UART0_MR                                     (*(volatile unsigned long*)(UART0_BASE_ADDR + 0x00000004u))
#define REG_UART0_IER                                    (*(volatile unsigned long*)(UART0_BASE_ADDR + 0x00000008u))
#define REG_UART0_IDR                                    (*(volatile unsigned long*)(UART0_BASE_ADDR + 0x0000000Cu))
#define REG_UART0_IMR                                    (*(volatile unsigned long*)(UART0_BASE_ADDR + 0x00000010u))
#define REG_UART0_ISR                                    (*(volatile unsigned long*)(UART0_BASE_ADDR + 0x00000014u))
#define REG_UART0_BAUDGEN                                (*(volatile unsigned long*)(UART0_BASE_ADDR + 0x00000018u))
#define REG_UART0_RXTOUT                                 (*(volatile unsigned long*)(UART0_BASE_ADDR + 0x0000001Cu))
#define REG_UART0_RXWM                                   (*(volatile unsigned long*)(UART0_BASE_ADDR + 0x00000020u))
#define REG_UART0_MODEMCR                                (*(volatile unsigned long*)(UART0_BASE_ADDR + 0x00000024u))
#define REG_UART0_MODEMSR                                (*(volatile unsigned long*)(UART0_BASE_ADDR + 0x00000028u))
#define REG_UART0_SR                                     (*(volatile unsigned long*)(UART0_BASE_ADDR + 0x0000002Cu))
#define REG_UART0_RXFIFO                                 (*(volatile unsigned long*)(UART0_BASE_ADDR + 0x00000030u))
#define REG_UART0_BDIV                                   (*(volatile unsigned long*)(UART0_BASE_ADDR + 0x00000034u))
#define REG_UART0_FDR                                    (*(volatile unsigned long*)(UART0_BASE_ADDR + 0x00000038u))
#define REG_UART0_TXFIFO_TRGLVL                          (*(volatile unsigned long*)(UART0_BASE_ADDR + 0x00000044u))

/****** Static Memory Controller ************************************/
#define MEMC_BASE_ADDR                                   (0xE0000000u)
#define REG_MEMC_CFG_SET                                 (*(volatile unsigned long*)(MEMC_BASE_ADDR + 0xE008u))

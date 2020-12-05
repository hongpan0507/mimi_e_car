/**************************************************************************
 * @file        DRV8343.h
 * @author      MDBU Software Team
 * @date        13th Novemeber 2017
 * @brief       This file defines all the functions and data structures used for accessing DRV8343 registers
 * @note        Copyright (c) 2017 Texas Instruments Incorporated.
 *              All rights reserved.
 ******************************************************************************/

#ifndef DRV8343_H_
#define DRV8343_H_

#include "dataTypeDefinition.h"

#define NUMCONFIG_IDRIVE_VALUES                 16       // Defines the number of configurable Idrive values for the DRv83xxS Devices
#define NUMCONFIG_TDRIVE_VALUES                 4       // Defines the number of configurable Idrive values for the DRv83xxS Devices

/*************** Idrive & Tdrive Configuration Settings ****************/
#define GATE_TO_DRAIN_CHARGE        8.7      // Set the gate to drain charge of the FET used on the EVM in uC
#define RISE_TIME                   100     // Select the Maximum Rise time desired for the FET in nano seconds
#define FALL_TIME                   50     // Select the Maximum Fall time desired for the FET in nano Seconds

/*************************************************************
* drv8343 Analog Subsystem SPI REGISTER ADDRESS
*************************************************************/

/* Analog Subsystem Instructions */
#define SPI_REG_FAULT_STAT      (0x00)         /*  */
#define SPI_REG_DIAG_STAT_A     (0x01)         /*  */
#define SPI_REG_DIAG_STAT_B     (0x02)         /*  */
#define SPI_REG_DIAG_STAT_C     (0x03)         /*  */
#define SPI_REG_DRV_CTRL_1      (0x04)         /*  */
#define SPI_REG_DRV_CTRL_2      (0x05)         /*  */
#define SPI_REG_DRV_CTRL_3      (0x06)         /*  */
#define SPI_REG_DRV_CTRL_4      (0x07)         /*  */
#define SPI_REG_DRV_CTRL_5      (0x08)         /*  */
#define SPI_REG_DRV_CTRL_6      (0x09)         /*  */
#define SPI_REG_DRV_CTRL_7      (0x0A)         /*  */
#define SPI_REG_DRV_CTRL_8      (0x0B)         /*  */
#define SPI_REG_DRV_CTRL_9      (0x0C)         /*  */
#define SPI_REG_DRV_CTRL_10     (0x0D)         /*  */
#define SPI_REG_DRV_CTRL_11     (0x0E)         /*  */
#define SPI_REG_DRV_CTRL_12     (0x0F)         /*  */
#define SPI_REG_DRV_CTRL_13     (0x10)         /*  */
#define SPI_REG_DRV_CTRL_14     (0x11)         /*  */
#define SPI_REG_DRV_CTRL_15     (0x12)         /*  */

/* Analog Subsystem Instructions - Bit Definitions */
/* DRV8343S SPI REGISTER Masked Bit fields*/
/* SPI_REG_00 : SPI_REG_FAULT_STAT_MASK  */
#define DRV8343S_FAULT_MASK       (0x80)         /*  */
#define DRV8343S_GDF_MASK         (0x40)         /*  */
#define DRV8343S_CPUV_MASK        (0x20)         /*  */
#define DRV8343S_UVLO_MASK        (0x10)         /*  */
#define DRV8343S_OCP_MASK         (0x08)         /*  */
#define DRV8343S_OTW_MASK         (0x04)         /*  */
#define DRV8343S_OTSD_MASK        (0x02)         /*  */
#define DRV8343S_OL_SHT_MASK      (0x01)         /*  */

/* SPI_REG_01 : SPI_REG_DIAG_STAT_A_MASK */
#define DRV8343S_SA_OC_MASK      (0x80)         /*  */
#define DRV8343S_SHT_GND_A_MASK  (0x40)         /*  */
#define DRV8343S_SHT_BAT_A_MASK  (0x20)         /*  */
#define DRV8343S_OL_PH_A_MASK    (0x10)         /*  */
#define DRV8343S_VGS_LA_MASK     (0x08)         /*  */
#define DRV8343S_VGS_HA_MASK     (0x04)         /*  */
#define DRV8343S_VDS_LA_MASK     (0x02)         /*  */
#define DRV8343S_VDS_HA_MASK     (0x01)         /*  */

/* SPI_REG_02 : SPI_REG_DIAG_STAT_B_MASK */
#define DRV8343S_SB_OC_MASK      (0x80)         /*  */
#define DRV8343S_SHT_GND_B_MASK  (0x40)         /*  */
#define DRV8343S_SHT_BAT_B_MASK  (0x20)         /*  */
#define DRV8343S_OL_PH_B_MASK    (0x10)         /*  */
#define DRV8343S_VGS_LB_MASK     (0x08)         /*  */
#define DRV8343S_VGS_HB_MASK     (0x04)         /*  */
#define DRV8343S_VDS_LB_MASK     (0x02)         /*  */
#define DRV8343S_VDS_HB_MASK     (0x01)         /*  */

/* SPI_REG_03 : SPI_REG_DIAG_STAT_C_MASK */
#define DRV8343S_SC_OC_MASK      (0x80)         /*  */
#define DRV8343S_SHT_GND_C_MASK  (0x40)         /*  */
#define DRV8343S_SHT_BAT_C_MASK  (0x20)         /*  */
#define DRV8343S_OL_PH_C_MASK    (0x10)         /*  */
#define DRV8343S_VGS_LC_MASK     (0x08)         /*  */
#define DRV8343S_VGS_HC_MASK     (0x04)         /*  */
#define DRV8343S_VDS_LC_MASK     (0x02)         /*  */
#define DRV8343S_VDS_HC_MASK     (0x01)         /*  */

/* SPI_REG_04 : DRV_CTRL_REG_1_MASK */
#define DRV8343S_CLR_FLT_MASK      (0x80)         /*  */
#define DRV8343S_PWM_MODE_MASK     (0x70)         /*  */
#define DRV8343S_PWM_MODE_6x_MASK  (0x00)         /*  */
#define DRV8343S_PWM_MODE_3x_MASK  (0x10)         /*  */
#define DRV8343S_PWM_MODE_1x_MASK  (0x20)         /*  */
#define DRV8343S_BRIDGE_ABC_MODE_MASK  		(0x30)         /*  */
#define DRV8343S_BRIDGE_AB_FET_C_MODE_MASK  (0x40)         /*  */
#define DRV8343S_BRIDGE_BC_FET_A_MODE_MASK  (0x50)         /*  */
#define DRV8343S_BRIDGE_A_FET_BC_MODE_MASK  (0x60)         /*  */
#define DRV8343S_FET_ABC_MODE_MASK  		(0x70)         /*  */
#define DRV8343S_ONE_PWM_COM_MASK  (0x08)         /*  */
#define DRV8343S_ONE_PWM_DIR_MASK  (0x04)         /*  */
#define DRV8343S_ONE_PWM_BRK_MASK  (0x03)         /*  */


/* SPI_REG_05 : DRV_CTRL_REG_2_MASK */
#define DRV8343S_OTSD_MODE_MASK      (0x80)         /*  */
#define DRV8343S_OLP_SHTS_DLY_MASK   (0x60)         /*  */
#define DRV8343S_EN_SHT_TST_MASK     (0x10)         /*  */
#define DRV8343S_EN_OLP_MASK         (0x08)         /*  */
#define DRV8343S_EN_OLA_C_MASK       (0x04)         /*  */
#define DRV8343S_EN_OLA_B_MASK       (0x02)         /*  */
#define DRV8343S_EN_OLA_A_MASK       (0x01)         /*  */

/* SPI_REG_06 : DRV_CTRL_REG_3_MASK */
#define DRV8343S_IDRIVEP_LA_MASK     (0xF0)         /*  */
#define DRV8343S_IDRIVEP_HA_MASK     (0x0F)         /*  */

/* SPI_REG_07 : DRV_CTRL_REG_4_MASK */
#define DRV8343S_IDRIVEP_LB_MASK     (0xF0)         /*  */
#define DRV8343S_IDRIVEP_HB_MASK     (0x0F)         /*  */

/* SPI_REG_08 : DRV_CTRL_REG_5_MASK */
#define DRV8343S_IDRIVEP_LC_MASK     (0xF0)         /*  */
#define DRV8343S_IDRIVEP_HC_MASK     (0x0F)         /*  */

/* SPI_REG_09 : DRV_CTRL_REG_6_MASK */
#define DRV8343S_VDS_LVL_LA_MASK     (0xF0)         /*  */
#define DRV8343S_VDS_LVL_HA_MASK     (0x0F)         /*  */

/* SPI_REG_0A : DRV_CTRL_REG_7_MASK */
#define DRV8343S_VDS_LVL_LB_MASK     (0xF0)         /*  */
#define DRV8343S_VDS_LVL_HB_MASK     (0x0F)         /*  */
#define DRV8343S_VDS_LVL_LB_1p88V     (0xF0)         /*  */
#define DRV8343S_VDS_LVL_HB_1p88V     (0x0F)         /*  */

/* SPI_REG_0B : DRV_CTRL_REG_8_MASK */
#define DRV8343S_VDS_LVL_LC_MASK     (0xF0)         /*  */
#define DRV8343S_VDS_LVL_HC_MASK     (0x0F)         /*  */

/* SPI_REG_0C : DRV_CTRL_REG_9_MASK  */
#define DRV8343S_COAST_MASK       (0x80)         /*  */
#define DRV8343S_TRETRY_MASK      (0x60)         /*  */
#define DRV8343S_TRETRY_2ms       (0x00)         /*  */
#define DRV8343S_TRETRY_4ms       (0x20)         /*  */
#define DRV8343S_TRETRY_6ms       (0x40)         /*  */
#define DRV8343S_TRETRY_8ms       (0x60)         /*  */
#define DRV8343S_DEAD_TIME_MASK   (0x18)         /*  */
#define DRV8343S_DEAD_TIME_500ns    (0x00)         /*  */
#define DRV8343S_DEAD_TIME_1000ns   (0x08)         /*  */
#define DRV8343S_DEAD_TIME_2000ns   (0x10)         /*  */
#define DRV8343S_DEAD_TIME_4000ns   (0x18)         /*  */
#define DRV8343S_TDRIVE_MAX_MASK  (0x04)         /*  */
#define DRV8343S_TDRIVE_MASK	  (0x03)         /*  */

/* SPI_REG_0D : DRV_CTRL_REG_10_MASK  */
#define DRV8343S_LOCK_MASK       (0xE0)         /*  */
#define DRV8343S_DIS_CPUV_MASK   (0x10)         /*  */
#define DRV8343S_DIS_GDF_MASK    (0x08)         /*  */
#define DRV8343S_OCP_DEG_MASK    (0x07)         /*  */
#define DRV8343S_OCP_DEG_20p5us  (0x07)         /*  */

/* SPI_REG_0E : DRV_CTRL_REG_11_MASK  */
#define DRV8343S_RSVD_REG_10_MASK  (0x80)         /*  */
#define DRV8343S_OTW_REP_MASK      (0x40)         /*  */
#define DRV8343S_CBC_MASK          (0x20)         /*  */
#define DRV8343S_DIS_VDS_C_MASK    (0x10)         /*  */
#define DRV8343S_DIS_VDS_B_MASK    (0x08)         /*  */
#define DRV8343S_DIS_VDS_A_MASK    (0x04)         /*  */
#define DRV8343S_OCP_MODE_MASK     (0x03)         /*  */
#define DRV8343S_OCP_MODE_OFF      (0x03)         /*  */
#define DRV8343S_OCP_MODE_REPORT         (0x02)         /*  */
#define DRV8343S_OCP_MODE_RETRY          (0x01)         /*  */
#define DRV8343S_OCP_MODE_LATCHED_FLT    (0x00)         /*  */

/* SPI_REG_0F : DRV_CTRL_REG_12_MASK  */
#define DRV8343S_LS_REF_MASK       (0x80)         /*  */
#define DRV8343S_CSA_FET_MASK      (0x40)         /*  */
#define DRV8343S_CSA_GAIN_C_MASK   (0x30)         /*  */
#define DRV8343S_CSA_GAIN_B_MASK   (0x0C)         /*  */
#define DRV8343S_CSA_GAIN_A_MASK   (0x03)         /*  */
#define DRV8343S_CSA_GAIN_MASK 	   (0x3F)         /*  */
#define DRV8343S_CSA_GAIN_5		   (0b000000)         /*  */
#define DRV8343S_CSA_GAIN_10	   (0b010101)         /*  */
#define DRV8343S_CSA_GAIN_20	   (0b101010)         /*  */
#define DRV8343S_CSA_GAIN_40	   (0b111111)         /*  */

/* SPI_REG_10 : DRV_CTRL_REG_13_MASK  */
#define DRV8343S_CAL_MODE_MASK      (0x80)         /*  */
#define DRV8343S_VREF_DIV_MASK      (0x40)         /*  */
#define DRV8343S_SEN_LVL_C_MASK     (0x30)         /*  */
#define DRV8343S_SEN_LVL_B_MASK     (0x0C)         /*  */
#define DRV8343S_SEN_LVL_A_MASK     (0x03)         /*  */

/* SPI_REG_11 : DRV_CTRL_REG_14_MASK  */
#define DRV8343S_RSVD_REG_14_MASK    (0xC0)         /*  */
#define DRV8343S_DIS_SEN_C_MASK      (0x20)         /*  */
#define DRV8343S_DIS_SEN_B_MASK      (0x10)         /*  */
#define DRV8343S_DIS_SEN_A_MASK      (0x08)         /*  */
#define DRV8343S_DIS_SEN_ABC_MASK      (0x38)         /*Use for disable/enable all three amplifier current sense  */
#define DRV8343S_DIS_SEN_ABC      (0x38)         /*  */
#define DRV8343S_CSA_CAL_C_MASK      (0x04)         /*  */
#define DRV8343S_CSA_CAL_B_MASK      (0x02)         /*  */
#define DRV8343S_CSA_CAL_A_MASK      (0x01)         /*  */

/* DRV8343S SPI REGISTER Default Values*/

/* SPI_REG_00 : SPI_REG_FAULT_STAT  */
#define DRV8343S_FAULT       (0x00)         /*  */
#define DRV8343S_GDF         (0x00)         /*  */
#define DRV8343S_CPUV        (0x00)         /*  */
#define DRV8343S_UVLO        (0x00)         /*  */
#define DRV8343S_OCP         (0x00)         /*  */
#define DRV8343S_OTW         (0x00)         /*  */
#define DRV8343S_OTSD        (0x00)         /*  */
#define DRV8343S_OL_SHT      (0x00)         /*  */

/* SPI_REG_01 : SPI_REG_DIAG_STAT_A */
#define DRV8343S_SA_OC      (0x00)         /*  */
#define DRV8343S_SHT_GND_A  (0x00)         /*  */
#define DRV8343S_SHT_BAT_A  (0x00)         /*  */
#define DRV8343S_OL_PH_A    (0x00)         /*  */
#define DRV8343S_VGS_LA     (0x00)         /*  */
#define DRV8343S_VGS_HA     (0x00)         /*  */
#define DRV8343S_VDS_LA     (0x00)         /*  */
#define DRV8343S_VDS_HA     (0x00)         /*  */


/* SPI_REG_02 : SPI_REG_DIAG_STAT_B */
#define DRV8343S_SB_OC      (0x00)         /*  */
#define DRV8343S_SHT_GND_B  (0x00)         /*  */
#define DRV8343S_SHT_BAT_B  (0x00)         /*  */
#define DRV8343S_OL_PH_B    (0x00)         /*  */
#define DRV8343S_VGS_LB     (0x00)         /*  */
#define DRV8343S_VGS_HB     (0x00)         /*  */
#define DRV8343S_VDS_LB     (0x00)         /*  */
#define DRV8343S_VDS_HB     (0x00)         /*  */


/* SPI_REG_03 : SPI_REG_DIAG_STAT_C */
#define DRV8343S_SC_OC      (0x00)         /*  */
#define DRV8343S_SHT_GND_C  (0x00)         /*  */
#define DRV8343S_SHT_BAT_C  (0x00)         /*  */
#define DRV8343S_OL_PH_C    (0x00)         /*  */
#define DRV8343S_VGS_LC     (0x00)         /*  */
#define DRV8343S_VGS_HC     (0x00)         /*  */
#define DRV8343S_VDS_LC     (0x00)         /*  */
#define DRV8343S_VDS_HC     (0x00)         /*  */

/* SPI_REG_04 : DRV_CTRL_REG_1 */
#define DRV8343S_CLR_FLT      (0x00)         /*  */
#define DRV8343S_PWM_MODE     (0x00)         /*  */
#define DRV8343S_ONE_PWM_COM  (0x00)         /*  */
#define DRV8343S_ONE_PWM_DIR  (0x00)         /*  */
#define DRV8343S_ONE_PWM_BRK  (0x00)         /*  */

/* SPI_REG_05 : DRV_CTRL_REG_2 */
#define DRV8343S_OTSD_MODE      (0x00)         /*  */
#define DRV8343S_OLP_SHTS_DLY   (0x02)         /*  */
#define DRV8343S_EN_SHT_TST     (0x00)         /*  */
#define DRV8343S_EN_OLP         (0x00)         /*  */
#define DRV8343S_EN_OLA_C       (0x00)         /*  */
#define DRV8343S_EN_OLA_B       (0x00)         /*  */
#define DRV8343S_EN_OLA_A       (0x00)         /*  */

/* SPI_REG_06 : DRV_CTRL_REG_3 */
#define DRV8343S_IDRIVEP_LA     (0x0F)         /*  */
#define DRV8343S_IDRIVEP_HA     (0x0F)         /*  */

/* SPI_REG_07 : DRV_CTRL_REG_4 */
#define DRV8343S_IDRIVEP_LB     (0x0F)         /*  */
#define DRV8343S_IDRIVEP_HB     (0x0F)         /*  */


/* SPI_REG_08 : DRV_CTRL_REG_5 */
#define DRV8343S_IDRIVEP_LC     (0x0F)         /*  */
#define DRV8343S_IDRIVEP_HC     (0x0F)         /*  */

/* SPI_REG_09 : DRV_CTRL_REG_6 */
#define DRV8343S_VDS_LVL_LA     (0x09)         /*  */
#define DRV8343S_VDS_LVL_HA     (0x09)         /*  */

/* SPI_REG_0A : DRV_CTRL_REG_7 */
#define DRV8343S_VDS_LVL_LB     (0x09)         /*  */
#define DRV8343S_VDS_LVL_HB     (0x09)         /*  */

/* SPI_REG_0B : DRV_CTRL_REG_8 */
#define DRV8343S_VDS_LVL_LC     (0x09)         /*  */
#define DRV8343S_VDS_LVL_HC     (0x09)         /*  */

/* SPI_REG_0C : DRV_CTRL_REG_9  */
#define DRV8343S_COAST       (0x00)         /*  */
#define DRV8343S_TRETRY      (0x01)         /*  */
#define DRV8343S_DEAD_TIME   (0x01)         /*  */
#define DRV8343S_TDRIVE_MAX  (0x01)         /*  */
#define DRV8343S_TDRIVE	     (0x03)         /*  */

/* SPI_REG_0D : DRV_CTRL_REG_10  */
#define DRV8343S_LOCK       (0x03)         /*  */
#define DRV8343S_DIS_CPUV   (0x00)         /*  */
#define DRV8343S_DIS_GDF    (0x00)         /*  */
#define DRV8343S_OCP_DEG    (0x01)         /*  */

/* SPI_REG_0E : DRV_CTRL_REG_11  */
#define DRV8343S_RSVD_REG_10  (0x00)         /*  */
#define DRV8343S_OTW_REP      (0x00)         /*  */
#define DRV8343S_CBC          (0x00)         /*  */
#define DRV8343S_DIS_VDS_C    (0x00)         /*  */
#define DRV8343S_DIS_VDS_B    (0x00)         /*  */
#define DRV8343S_DIS_VDS_A    (0x00)         /*  */
#define DRV8343S_OCP_MODE     (0x00)         /*  */

/* SPI_REG_0F : DRV_CTRL_REG_12  */
#define DRV8343S_LS_REF       (0x00)         /*  */
#define DRV8343S_CSA_FET      (0x00)         /*  */
#define DRV8343S_CSA_GAIN_C   (0x02)         /*  */
#define DRV8343S_CSA_GAIN_B   (0x02)         /*  */
#define DRV8343S_CSA_GAIN_A   (0x02)         /*  */

/* SPI_REG_10 : DRV_CTRL_REG_13  */
#define DRV8343S_CAL_MODE      (0x00)         /*  */
#define DRV8343S_VREF_DIV      (0x01)         /*  */
#define DRV8343S_SEN_LVL_C     (0x03)         /*  */
#define DRV8343S_SEN_LVL_B     (0x03)         /*  */
#define DRV8343S_SEN_LVL_A     (0x03)         /*  */

/* SPI_REG_11 : DRV_CTRL_REG_14  */
#define DRV8343S_RSVD_REG_14    (0x00)         /*  */
#define DRV8343S_DIS_SEN_C      (0x00)         /*  */
#define DRV8343S_DIS_SEN_B      (0x00)         /*  */
#define DRV8343S_DIS_SEN_A      (0x00)         /*  */
#define DRV8343S_CSA_CAL_C      (0x00)         /*  */
#define DRV8343S_CSA_CAL_B      (0x00)         /*  */
#define DRV8343S_CSA_CAL_A      (0x00)         /*  */


typedef struct REG_MAP_Obj
{
    uint16_t FAULT_STAT;
    uint16_t DIAG_STAT_A;
    uint16_t DIAG_STAT_B;
    uint16_t DIAG_STAT_C;
    uint16_t DRV_CTRL_IC1;
    uint16_t DRV_CTRL_IC2;
    uint16_t DRV_CTRL_IC3;
    uint16_t DRV_CTRL_IC4;
    uint16_t DRV_CTRL_IC5;
    uint16_t DRV_CTRL_IC6;
    uint16_t DRV_CTRL_IC7;
    uint16_t DRV_CTRL_IC8;
    uint16_t DRV_CTRL_IC9;
    uint16_t DRV_CTRL_IC10;
    uint16_t DRV_CTRL_IC11;
    uint16_t DRV_CTRL_IC12;
    uint16_t DRV_CTRL_IC13;
    uint16_t DRV_CTRL_IC14;
    uint16_t DRV_CTRL_IC15;
} REG_MAP_Obj;

typedef struct FLT_STAT_Obj   // All the objects of Fault status have the prefix of "FSO"
{
    uint8_t FSO_FAULT;      // bit 7
    uint8_t FSO_GDF;    	// bit 6
    uint8_t FSO_CPUV;       // bit 5
    uint8_t FSO_UVLO;       // bit 4
    uint8_t FSO_OCP;       	// bit 3
    uint8_t FSO_OTW;     	// bit 2
    uint8_t FSO_OTSD;     	// bit 1
    uint8_t FSO_OL_SHT;     // bit 0

} FLT_STAT_Obj;


typedef struct DIAG_STAT_A_Obj   // All the objects of Diagnosis statu for Phase A have the prefix of "DSA"
{
    uint8_t DSA_FAULT;      // bit 7
    uint8_t DSA_SA_OC;    	// bit 6
    uint8_t DSA_SHT_BAT_A; 	// bit 5
    uint8_t DSA_OL_PH_A;   	// bit 4
    uint8_t DSA_VGS_LA;    	// bit 3
    uint8_t DSA_VGS_HA;    	// bit 2
    uint8_t DSA_VDS_LA;    	// bit 1
    uint8_t DSA_VDS_HA;    	// bit 0

} DIAG_STAT_A_Obj;


typedef struct DIAG_STAT_B_Obj   // All the objects of Diagnosis statu for Phase B have the prefix of "DSB"
{
    uint8_t DSB_FAULT;      // bit 7
    uint8_t DSB_SB_OC;    	// bit 6
    uint8_t DSB_SHT_BAT_B; 	// bit 5
    uint8_t DSB_OL_PH_B;   	// bit 4
    uint8_t DSB_VGS_LB;    	// bit 3
    uint8_t DSB_VGS_HB;    	// bit 2
    uint8_t DSB_VDS_LB;    	// bit 1
    uint8_t DSB_VDS_HB;    	// bit 0

} DIAG_STAT_B_Obj;
typedef struct DIAG_STAT_C_Obj   // All the objects of Diagnosis statu for Phase C have the prefix of "DSC"
{
    uint8_t DSC_FAULT;      // bit 7
    uint8_t DSC_SC_OC;    	// bit 6
    uint8_t DSC_SHT_BAT_C; 	// bit 5
    uint8_t DSC_OL_PH_C;   	// bit 4
    uint8_t DSC_VGS_LC;    	// bit 3
    uint8_t DSC_VGS_HC;    	// bit 2
    uint8_t DSC_VDS_LC;    	// bit 1
    uint8_t DSC_VDS_HC;    	// bit 0

} DIAG_STAT_C_Obj;
typedef struct DRV_CTRL_IC1_Obj   // All the objects of DRV Control register IC1 has prefix of "DCI1"
{
    uint8_t DCI1_CLR_FLT;      	// bit 7
    uint8_t DCI1_PWM_MODE;    	// bit 6 : 4
    uint8_t DCI1_ONE_PWM_COM; 	// bit 3
    uint8_t DCI1_ONE_PWM_DIR;   // bit 2
    uint8_t DCI1_ONE_PWM_BRK;   // bit 1 : 0
} DRV_CTRL_IC1_Obj;

typedef struct DRV_CTRL_IC2_Obj   // All the objects of DRV Control register IC2 has prefix of "DCI2"
{
    uint8_t DCI2_OTSD_MODE;      	// bit 7
    uint8_t DCI2_OLP_SHTS_DLY;    	// bit 6 : 5
    uint8_t DCI2_EN_SHT_TST; 		// bit 4
    uint8_t DCI2_EN_OLP;   			// bit 3
    uint8_t DCI2_EN_OLA_C;   		// bit 2
    uint8_t DCI2_EN_OLA_B;   		// bit 1
    uint8_t DCI2_EN_OLA_A;   		// bit 0
} DRV_CTRL_IC2_Obj;
typedef struct DRV_CTRL_IC3_Obj   // All the objects of DRV Control register IC3 has prefix of "DCI3"
{
    uint8_t DCI3_IDRIVEP_LA;      	// bit 7 : 4
    uint8_t DCI3_IDRIVEP_HA;   		// bit 3 : 0
} DRV_CTRL_IC3_Obj;
typedef struct DRV_CTRL_IC4_Obj   // All the objects of DRV Control register IC4 has prefix of "DCI3"
{
    uint8_t DCI4_IDRIVEP_LB;      	// bit 7 : 4
    uint8_t DCI4_IDRIVEP_HB;   		// bit 3 : 0
} DRV_CTRL_IC4_Obj;

typedef struct DRV_CTRL_IC5_Obj   // All the objects of DRV Control register IC5 has prefix of "DCI3"
{
    uint8_t DCI5_IDRIVEP_LC;      	// bit 7 : 4
    uint8_t DCI5_IDRIVEP_HC;   		// bit 3 : 0
} DRV_CTRL_IC5_Obj;

typedef struct DRV_CTRL_IC6_Obj   // All the objects of DRV Control register IC6 has prefix of "DCI6"
{
    uint8_t DCI6_VDS_LA;      	// bit 7 : 4
    uint8_t DCI6_VDS_HA;   		// bit 3 : 0
} DRV_CTRL_IC6_Obj;
typedef struct DRV_CTRL_IC7_Obj   // All the objects of DRV Control register IC6 has prefix of "DCI7"
{
    uint8_t DCI7_VDS_LB;      	// bit 7 : 4
    uint8_t DCI7_VDS_HB;   		// bit 3 : 0
} DRV_CTRL_IC7_Obj;
typedef struct DRV_CTRL_IC8_Obj   // All the objects of DRV Control register IC6 has prefix of "DCI8"
{
    uint8_t DCI8_VDS_LC;      	// bit 7 : 4
    uint8_t DCI8_VDS_HC;   		// bit 3 : 0
} DRV_CTRL_IC8_Obj;
typedef struct DRV_CTRL_IC9_Obj   // All the objects of DRV Control register IC9 has prefix of "DCI6"
{
    uint8_t DCI9_COAST;      	// bit 7
    uint8_t DCI9_TRETRY;    	// bit 6 : 5
    uint8_t DCI9_DEAD_TIME; 	// bit 4 : 3
    uint8_t DCI9_TDRIVE_MAX;   	// bit 2
    uint8_t DCI9_TDRIVE;   		// bit 1 : 0

} DRV_CTRL_IC9_Obj;
typedef struct DRV_CTRL_IC10_Obj   // All the objects of DRV Control register IC10 has prefix of "DCI10"
{
    uint8_t DCI10_LOCK;      	// bit 7 : 5
    uint8_t DCI10_DIS_CPUV;    	// bit 4
    uint8_t DCI10_DIS_GDF; 		// bit 3
    uint8_t DCI10_OCP_DEG;   	// bit 2 : 0
} DRV_CTRL_IC10_Obj;

typedef struct DRV_CTRL_IC11_Obj   // All the objects of DRV Control register IC11 has prefix of "DCI11"
{
    uint8_t DCI11_RSVD_REG_10;  	// bit 7
    uint8_t DCI11_OTW_REP;    	// bit 6
    uint8_t DCI11_CBC; 			// bit 5
    uint8_t DCI11_DIS_VDS_C;   	// bit 4
    uint8_t DCI11_DIS_VDS_B;   	// bit 3
    uint8_t DCI11_DIS_VDS_A;   	// bit 2
    uint8_t DCI11_OCP_MODE;   	// bit 1 : 0

} DRV_CTRL_IC11_Obj;

typedef struct DRV_CTRL_IC12_Obj   // All the objects of DRV Control register IC12 has prefix of "DCI9"
{
    uint8_t DCI12_LS_REF;  	 // bit 7
    uint8_t DCI12_CSA_FET;    // bit 6
    uint8_t DCI12_CSA_GAIN_C; // bit 5 : 4
    uint8_t DCI12_CSA_GAIN_B; // bit 3 : 2
    uint8_t DCI12_CSA_GAIN_A; // bit 1 : 0
} DRV_CTRL_IC12_Obj;

typedef struct DRV_CTRL_IC13_Obj   // All the objects of DRV Control register IC13 has prefix of "DCI13"
{
    uint8_t DCI13_CAL_MODE;  	 // bit 7
    uint8_t DCI13_VREF_DIV;    // bit 6
    uint8_t DCI13_SEN_LVL_C; // bit 5 : 4
    uint8_t DCI13_SEN_LVL_B; // bit 3 : 2
    uint8_t DCI13_SEN_LVL_A; // bit 1 : 0
} DRV_CTRL_IC13_Obj;
typedef struct DRV_CTRL_IC14_Obj   // All the objects of DRV Control register IC14 has prefix of "DCI14"
{
    uint8_t DCI14_RSVD_REG_14;  	 // bit 7
    uint8_t DCI14_DIS_SEN_C;    // bit 6
    uint8_t DCI14_DIS_SEN_B; // bit 5 : 4
    uint8_t DCI14_DIS_SEN_A; // bit 3 : 2
    uint8_t DCI14_CSA_CAL_C; // bit 1 : 0
    uint8_t DCI14_CSA_CAL_B; // bit 1 : 0
    uint8_t DCI14_CSA_CAL_A; // bit 1 : 0
} DRV_CTRL_IC14_Obj;

typedef struct DRV_CTRL_IC15_Obj   // All the objects of DRV Control register IC14 has prefix of "DCI14"
{
    uint8_t DCI15_RSVD_REG_15;  	 // bit 7 : 0
} DRV_CTRL_IC15_Obj;

typedef struct DRV_Control_Obj
{
    uint16_t Idrive_Min_Value;  /* Indicates the IdriveP minimum value */
    uint16_t Tdrive_Max_Value;   /* Indicates the IdriveP maximum value */
    uint16_t Idrive_Setting;    /* Indicates the IdriveN maximum value */
    uint16_t Tdrive_Setting;     /* Indicates the IdriveP Setting value */
    uint16_t Register_Counter;
} DRV_Control_Obj;


void DRV83xx_reg_write(uint16_t *addr, uint16_t *erase_mask, uint16_t *write_mask, uint16_t *read_data, uint16_t *write_data);
void DRV83xx_FLT_CLR(uint16_t *SPI_addr, uint16_t *write_data, uint16_t *read_data);

/* DRV8343S IdriveP_HS/LS Config Values in mA */
#define DRV8343S_IdriveP_MODE0		1.5
#define DRV8343S_IdriveP_MODE1		3.5
#define DRV8343S_IdriveP_MODE2		5
#define DRV8343S_IdriveP_MODE3		10
#define DRV8343S_IdriveP_MODE4		15
#define DRV8343S_IdriveP_MODE5		50
#define DRV8343S_IdriveP_MODE6		60
#define DRV8343S_IdriveP_MODE7		65
#define DRV8343S_IdriveP_MODE8      200
#define DRV8343S_IdriveP_MODE9		210
#define DRV8343S_IdriveP_MODE10		260
#define DRV8343S_IdriveP_MODE11		265
#define DRV8343S_IdriveP_MODE12		735
#define DRV8343S_IdriveP_MODE13		800
#define DRV8343S_IdriveP_MODE14		935
#define DRV8343S_IdriveP_MODE15		1000


/* DRV8343S Tdrive_HS/LS Config Values in nS */
#define DRV8343S_Tdrive_MODE0		500
#define DRV8343S_Tdrive_MODE1		1000
#define DRV8343S_Tdrive_MODE2		2000
#define DRV8343S_Tdrive_MODE3		4000



/////////////////////////
#endif /*DRV8343_H_*/

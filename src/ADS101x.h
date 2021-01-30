// library for 12-bit ADC (ADS101x) from TI
// Created on 1/17/2021 by hpan

#ifndef ADS101x_H_
#define ADS101x_H_

#include <stdint.h>

#define ADS101x_reg_byte_len		2	//all of the registers are 2 byte long except register address pointer
#define ADS101x_TX_buff_byte_len	3	//1 byte for addr ptr, 2 byte for write data
#define ADS101x_max_volt			3.3	//ADC max volt = ADC supply volt

enum I2C_addr{
	ADS101x_I2C_addr_EXT = 0b01001000,	//address for ADC connected to external interface	
	ADS101x_I2C_addr_INT = 0b01001001,	//address for ADC connected to internal measurement nodes
} ADS101x_I2C_addr;

enum reg_addr_ptr{	//DS page 23
	ADS101x_addr_ptr_byte_len = 1,	//1 byte of length, DS page 21
	ADS101x_conv_reg_ptr = 0b00,
	ADS101x_config_reg_ptr = 0b01,
	ADS101x_LO_th_reg_ptr = 0b10,
	ADS101x_HI_th_reg_ptr = 0b11
};

enum ADS101x_config_reg_OS{	//DS page 24
	ADS101x_OS_bit_loc = 15,
	ADS101x_OS_mask = 1 << ADS101x_OS_bit_loc,
	ADS101x_single_conv = 1 << ADS101x_OS_bit_loc 
};

enum ADS1015_config_reg_MUX{	//DS page 24
	ADS1015_MUX_bit_loc = 12,
	ADS1015_ADC_channel_count = 4,
	ADS1015_MUX_mask = 0b111 << ADS1015_MUX_bit_loc,
	ADS1015_MUX_DIFF_P_IN0_N_IN1 = 0b000 << ADS1015_MUX_bit_loc,	//default
	ADS1015_MUX_DIFF_P_IN0_N_IN3 = 0b001 << ADS1015_MUX_bit_loc,
	ADS1015_MUX_DIFF_P_IN1_N_IN3 = 0b010 << ADS1015_MUX_bit_loc,
	ADS1015_MUX_DIFF_P_IN2_N_IN3 = 0b011 << ADS1015_MUX_bit_loc,
	ADS1015_MUX_SE_IN0 = 0b100 << ADS1015_MUX_bit_loc,
	ADS1015_MUX_SE_IN1 = 0b101 << ADS1015_MUX_bit_loc,
	ADS1015_MUX_SE_IN2 = 0b110 << ADS1015_MUX_bit_loc,
	ADS1015_MUX_SE_IN3 = 0b111 << ADS1015_MUX_bit_loc
};

enum ADS101x_config_reg_PGA{
	ADS101x_PGA_FSR_bit_loc = 9,
	ADS101x_PGA_FSR_mask = 0b111 << ADS101x_PGA_FSR_bit_loc,
	ADS101x_PGA_FSR_PM_6p144V = 0b000 << ADS101x_PGA_FSR_bit_loc,
	ADS101x_PGA_FSR_PM_4p096V = 0b001 << ADS101x_PGA_FSR_bit_loc,
	ADS101x_PGA_FSR_PM_2p048V = 0b010 << ADS101x_PGA_FSR_bit_loc,	//default
	ADS101x_PGA_FSR_PM_1p024V = 0b011 << ADS101x_PGA_FSR_bit_loc,
	ADS101x_PGA_FSR_PM_0p512V = 0b100 << ADS101x_PGA_FSR_bit_loc,
	ADS101x_PGA_FSR_PM_0p256V = 0b101 << ADS101x_PGA_FSR_bit_loc	//the rest all sets FSR to +/-0.256V
};

enum ADS101x_config_reg_MODE{
	ADS101x_MODE_bit_loc = 8,
	ADS101x_MODE_mask = 1 << ADS101x_MODE_bit_loc,
	ADS101x_MODE_CC = 0 << ADS101x_MODE_bit_loc,	//continuous conversion
	ADS101x_MODE_SS = 1 << ADS101x_MODE_bit_loc		//single shot; default
};

enum ADS101x_config_reg_DR{	//ADC conversion Data Rate
	ADS101x_DR_bit_loc = 5,
	ADS101x_DR_mask = 0b111 << ADS101x_DR_bit_loc, 
	ADS101x_DR_128SPS = 0b000 << ADS101x_DR_bit_loc,
	ADS101x_DR_250SPS = 0b001 << ADS101x_DR_bit_loc,
	ADS101x_DR_490SPS = 0b010 << ADS101x_DR_bit_loc,
	ADS101x_DR_920SPS = 0b011 << ADS101x_DR_bit_loc,
	ADS101x_DR_1600SPS = 0b100 << ADS101x_DR_bit_loc,	//default
	ADS101x_DR_2400SPS = 0b101 << ADS101x_DR_bit_loc,
	ADS101x_DR_3300SPS = 0b110 << ADS101x_DR_bit_loc	// the rest is the same as 3300SPS
};

enum ADS101x_config_reg_COMP_MODE{
	ADS101x_COMP_MODE_loc = 4,
	ADS101x_COMP_MODE_mask = 1 << ADS101x_COMP_MODE_loc,
	ADS101x_COMP_MODE_TC = 0 << ADS101x_COMP_MODE_loc,	//traditional comparator; default
	ADS101x_COMP_MODE_WC = 1 << ADS101x_COMP_MODE_loc	//window comparator
};

enum ADS101x_config_reg_COMP_POL{	//controls ALERT/RDY PIN polarity
	ADS101x_COMP_POL_bit_loc = 3,
	ADS101x_COMP_POL_mask = 1 << ADS101x_COMP_POL_bit_loc,
	ADS101x_COMP_POL_ACT_LO = 0 << ADS101x_COMP_POL_bit_loc,	//active low; default
	ADS101x_COMP_POL_ACT_HI = 1 << ADS101x_COMP_POL_bit_loc		//active high 
};

enum ADS101x_config_reg_COMP_LAT{	//controls ALERT/RDY PIN latching behavior
	ADS101x_COMP_LAT_bit_loc = 2,
	ADS101x_COMP_LAT_mask = 1 << ADS101x_COMP_LAT_bit_loc,
	ADS101x_COMP_LAT_OFF = 0 << ADS101x_COMP_LAT_bit_loc,		//default
	ADS101x_COMP_LAT_ON = 1 << ADS101x_COMP_LAT_bit_loc
};

enum ADS101x_config_reg_COMP_QUE{	
	ADS101x_COMP_QUE_bit_loc = 0,
	ADS101x_COMP_QUE_mask = 0b11,
	ADS101x_COMP_QUE_1conv = 0b00,
	ADS101x_COMP_QUE_2conv = 0b01,
	ADS101x_COMP_QUE_4conv = 0b10,
	ADS101x_COMP_OFF = 0b11			//default
};

struct ADS101x_para_obj{
	char reg_addr_ptr; 
	char TRX_buff[ADS101x_reg_byte_len];						//temporary data storage for TX and RX data
	uint16_t reg_mask;
	uint16_t reg_val;
	uint16_t ADC_data;
	float ADC_FSR;
	float ADC_volt_read;		//total 4 channels for ADS1015
};

void ADS101x_init(uint16_t I2C_addr);
void ADS101x_set_I2C_addr(enum I2C_addr *ADS101x_slave_addr);
void ADS101x_set_reg_addr_ptr(char *reg_addr_ptr);
void ADS101x_read_reg(char *reg_addr_ptr, char *read_data, uint8_t reg_data_byte_len);
void ADS101x_write_reg(char *reg_addr_ptr, char *write_data, uint8_t reg_data_byte_len);
void ADS101x_config_reg_reset(struct ADS101x_para_obj *ADS101x_para);
void ADS101x_config_reg_write(struct ADS101x_para_obj *ADS101x_para, uint16_t *reg_val_write);
void ADS101x_config_reg_read(struct ADS101x_para_obj *ADS101x_para);
void ADS101x_FSR_read(struct ADS101x_para_obj *ADS101x_para);
void ADS101x_single_conv_read(struct ADS101x_para_obj *ADS101x_para, uint16_t *ADC_channel);
void ADS101x_single_volt_read(struct ADS101x_para_obj *ADS101x_para, uint16_t *ADC_channel);
void ADS1015_set_ADC_channel(struct ADS101x_para_obj *ADS101x_para, uint16_t *ADC_channel);

#endif

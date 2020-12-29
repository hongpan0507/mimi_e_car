// TI TMP275 temperature sensor
#define tmp275_tmp_low_limit			47.0		// in C
#define tmp275_tmp_high_limit			50.0		// in C
#define I2C1_tmp275_slave_addr		0b01001001
#define tmp275_tmp_data_len			2

#define tmp275_tmp_conv_ceiling		127.9375	// in C; DS page 8	
#define tmp275_12bit_max			0x7FF		// max possible number by a signed 12-bit format	
#define tmp275_tmp_reg_byte2_mask	0xFFF0		// erase un-used bits; DS page 16
#define tmp275_tmp_reg_byte2_shift	4			// after erase, need to shift right by 4 bit; DS page 16

// Pointer Address; DS page 15
#define tmp275_tmp_data_reg			0b00
#define tmp275_config_reg			0b01
#define tmp275_tmp_low_reg			0b10
#define tmp275_tmp_high_reg			0b11
#define tmp275_ptr_byte_len 		1			// use for BCM2835 I2C write operation

// Configuration Register; DS page 16
#define tmp275_config_reg_erase_all			0xFF
#define tmp275_config_OneShot_mask			0b10000000
#define tmp275_config_OneShot_read			0b10000000
#define tmp275_config_POL_mask				0b00000100
#define tmp275_config_POL_active_high		0b00000100
#define tmp275_config_POL_active_low		0b00000000
#define tmp275_config_TM_mask				0b00000010
#define tmp275_config_TM_COMP				0b00000000
#define tmp275_config_TM_INT				0b00000010
#define tmp275_config_shutdown_mask			0b00000001
#define tmp275_config_shutdown_ON			0b00000001
#define tmp275_config_shutdown_OFF			0b00000000
#define tmp275_config_reg_byte_len 		1			

// Fault Queue; DS page 17
#define tmp275_config_FaultQueue_mask		0b00011000
#define tmp275_config_FaultQueue_1			0b00000000
#define tmp275_config_FaultQueue_2			0b00001000
#define tmp275_config_FaultQueue_4			0b00010000
#define tmp275_config_FaultQueue_6			0b00011000

// Converter resolution; DS page 17
#define tmp275_resolution_reg_mask	0b01100000
#define tmp275_resolution_9bit		0b00000000
#define tmp275_resolution_10bit		0b00100000
#define tmp275_resolution_11bit		0b01000000
#define tmp275_resolution_12bit		0b01100000

void tmp275_init();
void tmp275_tmp_report(float *PCB_tmp_C, float *PCB_tmp_F, short *tmp_DAC_data);
void *tmp275_read_tmp(short *data, float *tmp);
void tmp275_set_tmp_limit(char *tmp275_ptr, float *tmp_limit);
void tmp275_write_ptr(char *tmp275_ptr);
void tmp275_write_reg(char *tmp275_ptr, char *reg_data, int reg_data_byte_len);
void tmp275_read_reg(char *tmp275_ptr, char *reg_data, int reg_data_byte_len);
void char_buff_to_short(char *buff, short *data);
void data_to_tmp(short *tmp_data, float *tmp);
void tmp_to_data(char *tmp_DAC_data, float *tmp);

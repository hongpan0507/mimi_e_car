// TI TMP275 temperature sensor

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

void tmp275_init();
float *tmp275_read_tmp(short *data);
void tmp275_write_ptr(char *tmp275_ptr);
void char_buff_to_short(char *buff, short *data);
void data_to_tmp(short *tmp_data, float *tmp);

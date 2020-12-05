#include "tmp275.h"
#include <bcm2835.h>
#include "utilities.h"
#include <stdio.h>

void tmp275_init(){
//	uint8_t tmp275_addr = I2C1_tmp275_slave_addr;
//	bcm2835_i2c_setSlaveAddress(tmp275_addr);
	bcm2835_i2c_setSlaveAddress(I2C1_tmp275_slave_addr);	//set I2C address to communicate with tmp275

	char tmp275_ptr;
	//point to temperature register before reading temperature
	tmp275_ptr = tmp275_tmp_data_reg;
	tmp275_write_ptr(&tmp275_ptr);	
	
}

// Datasheet Page 8
// TMP275 temperature data is transfered as 2 byte data 
// Byte 1, the most significant byte, is transfered first; it is followed by byte 2
// Only first 12-bit contains temperature data, the rest is zero
// In this case, negative temperature is of interest; the data is stored as 2's complement
// The best option for storaging the temperature data after it's read might be "short"
// data_byte_len = 2 for reading temperature data from tmp275
// Debugging tips:
// Make sure I2C has the address set to read tmp275 chip
// Make sure tmp275 pointer is pointing to temperature register before reading
float *tmp275_read_tmp(short *data){
	// define as static in order to return its pointer position
	// static keeps the variable alive after function call
	static float tmp = 0;	
	static char tmp275_buff[tmp275_tmp_data_len];

//	printf("Data before I2C read: %d \n", tmp275_buff[0]);
//	printf("Data before I2C read: %d \n", tmp275_buff[1]);
	bcm2835_i2c_read(tmp275_buff, tmp275_tmp_data_len);	// read temperature using I2C
//	printf("Data after I2C read: %d \n", tmp275_buff[0]);
//	printf("Data after I2C read: %d \n", tmp275_buff[1]);
	
	char_buff_to_short(tmp275_buff, data);
	//printf("Data after buff conversion to short: %d \n", *data);
	data_to_tmp(data, &tmp);
	//printf("Data after temp conversion: %.1f \n", tmp);
	return &tmp;
}


void tmp275_write_ptr(char *tmp275_ptr){
	bcm2835_i2c_write(tmp275_ptr, tmp275_ptr_byte_len);	// write tmp275 pointer using I2C
}

// convert char buff[2] received by I2C to short
// each char is 8 bits, each short is 16 bits, char array cannot be greater than 2
void char_buff_to_short(char *buff, short *data){
	*data = 0;	// make sure the content is zero before using it because bitwise OR is senitive to it
	*data = *data | buff[0];	//copy the most significant byte
	*data = *data << 8; //left shift the most signficiant byte by 8 bit
	*data = *data | buff[1];	//copy the least significant byte
	// from DS page 16; the last 4 bit of Byte 2 is not used for temperature data--> erase it
	// need to right shift by 4 bit to get the temperature data to the correct bit field
	*data = *data & tmp275_tmp_reg_byte2_mask;
	*data = *data >> tmp275_tmp_reg_byte2_shift;
}

void data_to_tmp(short *tmp_data, float *tmp){
	// printf("temperature raw data in decimal: %d \n", *tmp_data);

	// convert tmp_data from int to float before temperature conversion calculation
	// if not convertint to float, output of DAC_data/Full_Range will be severely affected
	signed_12bit_to_signed_16bit(tmp_data);
	*tmp = (float)*tmp_data/tmp275_12bit_max * tmp275_tmp_conv_ceiling;
	//printf("TMP275 Temperature Reading: %.1fC\n", *tmp);
}

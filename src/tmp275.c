#include "tmp275.h"
#include <bcm2835.h>
#include "utilities.h"
#include <stdio.h>

void tmp275_init(){
	printf("Initializing tmp275 configuration... \n");
	bcm2835_i2c_setSlaveAddress(I2C1_tmp275_slave_addr);	//set I2C address to communicate with tmp275

	char tmp275_ptr;
	char tmp275_reg_data;
	short tmp_DAC_data = 0; 
	float tmp_limit = 0;
	float *PCB_tmp_C;		// PCB temperature data read from TMP275 closest to the MOSFETs
	float *PCB_tmp_F;		// PCB temperature data read from TMP275 closest to the MOSFETs

	printf("before writing config register \n");
	tmp275_ptr = tmp275_config_reg;
	tmp275_read_reg(&tmp275_ptr, &tmp275_reg_data, tmp275_config_reg_byte_len);	// read current config reg data
	printf("print config reg data as binary: ");
	tmp_DAC_data = (short) tmp275_reg_data;
	print_as_bin(&tmp_DAC_data);

	//set up converter precision
	tmp275_reg_data &= ~(tmp275_resolution_reg_mask);	//erase current resolution setting
	tmp275_reg_data |= tmp275_resolution_9bit;		// write desired resolution
	//set up alter pin polarity; set to active high
	tmp275_reg_data &= ~(tmp275_config_POL_mask);	
	tmp275_reg_data |= tmp275_config_POL_active_high;

	tmp275_write_reg(&tmp275_ptr, &tmp275_reg_data, tmp275_config_reg_byte_len);

	printf("After writing config register \n");
	tmp275_read_reg(&tmp275_ptr, &tmp275_reg_data, tmp275_config_reg_byte_len);	// read current config reg data
	printf("print config reg data as binary: ");
	tmp_DAC_data = (short) tmp275_reg_data;
	print_as_bin(&tmp_DAC_data);

	//default temperature limit on power on: low=75C, high=80C
	//set up low and high temperature limit 
	printf("setting temperature limit \n");
	tmp275_ptr = tmp275_tmp_low_reg;
	tmp_limit = tmp275_tmp_low_limit;
	tmp275_set_tmp_limit(&tmp275_ptr, &tmp_limit);
	PCB_tmp_C = tmp275_read_tmp(&tmp_DAC_data);
	PCB_tmp_F = C_to_F(PCB_tmp_C);
	printf("Lower Temperature Limit = %.1fC ", *PCB_tmp_C);
	printf("(%.1fF) \n", *PCB_tmp_F);

	tmp275_ptr = tmp275_tmp_high_reg;
	tmp_limit = tmp275_tmp_high_limit;
	tmp275_set_tmp_limit(&tmp275_ptr, &tmp_limit);
	PCB_tmp_C = tmp275_read_tmp(&tmp_DAC_data);
	PCB_tmp_F = C_to_F(PCB_tmp_C);
	printf("Upper Temperature Limit = %.1fC ", *PCB_tmp_C);
	printf("(%.1fF) \n", *PCB_tmp_F);

	//point to temperature register before reading temperature
	tmp275_ptr = tmp275_tmp_data_reg;
	tmp275_write_ptr(&tmp275_ptr);	
	PCB_tmp_C = tmp275_read_tmp(&tmp_DAC_data);
	PCB_tmp_F = C_to_F(PCB_tmp_C);

	printf("PCB Temperature = %.1fC ", *PCB_tmp_C);
	printf("(%.1fF) \n", *PCB_tmp_F);

	printf("tmp275 temperature precision: \n");
	printf("tmp275 upper temperature \n");
	printf("tmp275 default pointer is set to read temperature\n");
	printf("tmp275 setup completed\n");	
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

//setting high or low temperature limit for TMP275
void tmp275_set_tmp_limit(char *tmp275_ptr, float *tmp_limit){
	//printf("tmp limit 1: %.1fC \n", *tmp_limit);
	char tmp_DAC_data[2];
	tmp_to_data(tmp_DAC_data, tmp_limit);	// convert from temperature limit from float to char
	tmp275_write_reg(tmp275_ptr, tmp_DAC_data, sizeof(tmp_DAC_data)/sizeof(tmp_DAC_data[0]));
}

// write tmp275 address pointer
void tmp275_write_ptr(char *tmp275_ptr){
	bcm2835_i2c_write(tmp275_ptr, tmp275_ptr_byte_len);	// write tmp275 pointer using I2C
}

// generic function for reading tmp275 register raw data 
// do not use this function to read temperature register
// this function repeatly set tmp275 pointer address which is not necessary 
// if read reg is a temperature register
void tmp275_read_reg(char *tmp275_ptr, char *reg_data, int reg_data_byte_len){
	tmp275_write_ptr(tmp275_ptr);	// set address pointer
	bcm2835_i2c_read(reg_data, reg_data_byte_len);	// read temperature using I2C

}

// DS page 12, Need to write reg data right after sending pointer address
// this means sending pointer and reg data using 2 I2C command is not okay
// because each I2C command has slave address preceeding I2C data
void tmp275_write_reg(char *tmp275_ptr, char *reg_data, int reg_data_byte_len){
	char data[3];	// tmp275 requires 2 byte of reg_data at the most, 1 byte is for holding pointer address
	data[0] = *tmp275_ptr;	//1st byte is the pointer address
	// the rest contains reg_data
	for(int i=1; i<=reg_data_byte_len; i++){
		data[i] = reg_data[i-1];
	}
	//printf("Data to be sent: ");
	//print_char_array(data, sizeof(data));
	bcm2835_i2c_write(data, reg_data_byte_len+1);	// +1 to include address pointer
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

// convert short to char data[2] for writing temperature limit to the register
// each char is 8 bits, each short is 16 bits, char array cannot be greater than 2
// no need to do anything special to a negative number 
// because 16bit to 12bit conversion takes care of negative number stored in 2s complement after bit shifting to the right
void short_to_char_buff(char *data, short *tmp_limit){
	data[0] = (char) (*tmp_limit >> 4); //right shift tmp_limit by 4 to get most significant 8 bit into char [0]
	data[1] = (char) (*tmp_limit << 4); //left shift tmp_limit by 4 to get least significant 8 bit into char [1]
}

void data_to_tmp(short *tmp_data, float *tmp){
	// printf("temperature raw data in decimal: %d \n", *tmp_data);

	// convert tmp_data from int to float before temperature conversion calculation
	// if not convertint to float, output of DAC_data/Full_Range will be severely affected
	signed_12bit_to_signed_16bit(tmp_data);
	*tmp = (float)*tmp_data/tmp275_12bit_max * tmp275_tmp_conv_ceiling;
	//printf("TMP275 Temperature Reading: %.1fC\n", *tmp);
}

// convert tmp to data that TMP275 understands
void tmp_to_data(char *tmp_DAC_data, float *tmp){
	short tmp_data = 0;
	//printf("tmp limit 2: %.1fC \n", *tmp);
	tmp_data = *tmp * tmp275_12bit_max/tmp275_tmp_conv_ceiling;
	//check DS page 18 for data format
	tmp_DAC_data[0] = (char) (tmp_data >> 4); //right shift tmp_limit by 4 to get most significant 8 bit into char [0]
	tmp_DAC_data[1] = (char) (tmp_data << 4); //left shift tmp_limit by 4 to get least significant 8 bit into char [1]
	//printf("tmp limit char0: %d \n", (short)tmp_DAC_data[0]);
	//printf("tmp limit char1: %d \n", (short)tmp_DAC_data[1]);
	//printf("converted temperature raw data in decimal: %d \n", tmp_data);
 	//*tmp = (float)tmp_data/tmp275_12bit_max * tmp275_tmp_conv_ceiling;
 	//printf("Temperature Limit Setting: %.1fC\n", *tmp);
	//char_buff_to_short(tmp_DAC_data, &tmp_data);
	//printf("feedback data in decimal: %d \n", tmp_data);
}

// library for 12-bit ADC (ADS101x) from TI
// Created on 1/17/2021 by hpan

#include "ADS101x.h"
#include "utilities.h"
#include "TI_utilities.h"
#include <bcm2835.h>
#include <stdio.h>

void ADS101x_init(){
	printf("------------------------------- \n");
	printf("Initializing ADC parameters... \n");
	uint16_t reg_val_write;
	uint16_t reg_mask;
	static struct ADS101x_para_obj ADS101x_para = {
		.reg_addr_ptr = ADS101x_conv_reg_ptr	//point to ADC conversion register
	}; 

	ADS101x_I2C_addr = ADS101x_I2C_addr_EXT;
	ADS101x_set_I2C_addr(&ADS101x_I2C_addr);

	printf("Setting ADC Full Scale Range \n");
	reg_val_write = ADS101x_PGA_FSR_PM_4p096V;
	reg_mask = ADS101x_PGA_FSR_mask;
	ADS101x_config_reg_write(&ADS101x_para, &reg_val_write, &reg_mask);
	ADS101x_FSR_read(&ADS101x_para);

	printf("Set ADC channel to 3, single ended \n");
	reg_val_write = ADS1015_MUX_SE_IN3;
	reg_mask = ADS1015_MUX_mask;
	ADS101x_config_reg_write(&ADS101x_para, &reg_val_write, &reg_mask);

	//reading current config register data
	ADS101x_para.reg_addr_ptr = ADS101x_config_reg_ptr;	//point to ADC config register
	ADS101x_read_reg(&(ADS101x_para.reg_addr_ptr), ADS101x_para.TRX_buff, ADS101x_reg_byte_len);
	_2char_to_16bit(ADS101x_para.TRX_buff, &(ADS101x_para.reg_val));
	printf("config register data in binary: ");
	print_as_bin(&(ADS101x_para.reg_val));

	//start ADC single conversion
	while(1){
		//ADS101x_single_conv_read(&ADS101x_para);
		ADC_volt_single_conv_read(&ADS101x_para);
		delay(100);
	}
	
	printf("ADC setup completed \n");
	printf("------------------------------- \n");
}

void ADS101x_set_I2C_addr(enum I2C_addr *ADS101x_slave_addr){
	printf("ADS101x I2C addr = %d \n", *ADS101x_slave_addr);
	bcm2835_i2c_setSlaveAddress(*ADS101x_slave_addr);	//set I2C address to communicate with tmp275
}

void ADS101x_set_reg_addr_ptr(char *reg_addr_ptr){
	bcm2835_i2c_write(reg_addr_ptr, ADS101x_addr_ptr_byte_len);	// write tmp275 pointer using I2C
}

void ADS101x_read_reg(char *reg_addr_ptr, char *read_data, uint8_t reg_data_byte_len){
	ADS101x_set_reg_addr_ptr(reg_addr_ptr);
	bcm2835_i2c_read(read_data, reg_data_byte_len);	// read temperature using I2C
}

void ADS101x_write_reg(char *reg_addr_ptr, char *write_data, uint8_t reg_data_byte_len){
	//requires 2 byte of reg_data at the most, 1 byte is for holding pointer address
	char data[3];
	data[0] = *reg_addr_ptr;	//1st byte is the pointer address
	// the rest contains reg_data
	for(int i=1; i<=reg_data_byte_len; i++){
		data[i] = write_data[i-1];
	}
	//printf("Data to be sent: ");
	//print_char_array(data, sizeof(data));
	bcm2835_i2c_write(data, reg_data_byte_len+1);	// +1 to include address pointer
}

void ADS101x_config_reg_reset(struct ADS101x_para_obj *ADS101x_para){
	printf("Resetting ADS101x Config Reg Content \n");
	ADS101x_para->reg_addr_ptr = ADS101x_config_reg_ptr;	//point to ADC config register
	ADS101x_para->reg_val = 0x8583;	//default value, DS p24
	//printf("default config reg: ");
	//print_as_bin(&(ADS101x_para->reg_val));
	_16bit_to_2char(ADS101x_para->TRX_buff, &(ADS101x_para->reg_val));	//convert short back to char[2] for I2C communication
	ADS101x_write_reg(&(ADS101x_para->reg_addr_ptr), ADS101x_para->TRX_buff, ADS101x_reg_byte_len);
}

//reg_val = reg value to be written to config reg
//bit_loc = the number of bit shift required to get the reg_val into the correct location of the config reg bit field
//reg_mask = use to erase current bit field data
void ADS101x_config_reg_write(struct ADS101x_para_obj *ADS101x_para, uint16_t *reg_val_write, uint16_t *reg_mask){
	ADS101x_para->reg_addr_ptr = ADS101x_config_reg_ptr;	//point to ADC config register
	ADS101x_read_reg(&(ADS101x_para->reg_addr_ptr), ADS101x_para->TRX_buff, ADS101x_reg_byte_len);	//read config register
	_2char_to_16bit(ADS101x_para->TRX_buff, &(ADS101x_para->reg_val));	//convert char[2] to short for data processing
	ADS101x_para->reg_val = ADS101x_para->reg_val & (~ *reg_mask);	//bitwise invert reg_mask to erase specific bit field data
	ADS101x_para->reg_val = ADS101x_para->reg_val | *reg_val_write;	//bitwise invert reg_mask to erase specific bit field data
	_16bit_to_2char(ADS101x_para->TRX_buff, &(ADS101x_para->reg_val));	//convert short back to char[2] for I2C communication
	ADS101x_write_reg(&(ADS101x_para->reg_addr_ptr), ADS101x_para->TRX_buff, ADS101x_reg_byte_len);		//put the data back to the config register
}


void ADS101x_config_reg_read(struct ADS101x_para_obj *ADS101x_para, uint16_t reg_mask){
	ADS101x_para->reg_addr_ptr = ADS101x_config_reg_ptr;	//point to ADC config register
	ADS101x_read_reg(&(ADS101x_para->reg_addr_ptr), ADS101x_para->TRX_buff, ADS101x_reg_byte_len);	//read config register
	_2char_to_16bit(ADS101x_para->TRX_buff, &(ADS101x_para->reg_val));	//convert char[2] to short for data processing
	ADS101x_para->reg_val = ADS101x_para->reg_val & reg_mask;	//erase all the bit field except the ones defined by reg_mask
}

//this function finds the full scale range based on PGA setting
void ADS101x_FSR_read(struct ADS101x_para_obj *ADS101x_para){
	ADS101x_config_reg_read(ADS101x_para, ADS101x_PGA_FSR_mask);
	switch(ADS101x_para->reg_val){
		case ADS101x_PGA_FSR_PM_6p144V:
			ADS101x_para->ADC_FSR = 6.144; 
			break;
		case ADS101x_PGA_FSR_PM_4p096V: 
			ADS101x_para->ADC_FSR = 4.096; 
			break;
		case ADS101x_PGA_FSR_PM_2p048V:
			ADS101x_para->ADC_FSR = 2.048; 
			break;
		case ADS101x_PGA_FSR_PM_1p024V:
			ADS101x_para->ADC_FSR = 1.024; 
			break;
		case ADS101x_PGA_FSR_PM_0p512V: 
			ADS101x_para->ADC_FSR = 0.512; 
			break;
		case ADS101x_PGA_FSR_PM_0p256V:
			ADS101x_para->ADC_FSR = 0.256; 
			break;
		default:
			ADS101x_para->ADC_FSR = -1000; //hopefully someone realizes that something is really wrong when he/she uses this number
			printf("***ERROR*** \n");
			printf("ADC Full Scale Range cannot be determined \n");
	}
	printf("ADS101x ADC Full Scale Range = %.3fV \n", ADS101x_para->ADC_FSR);
}

//start single conversion and read ADC data
//read config register; OR "1" to bit 16, then put the data back to the config register
void ADS101x_single_conv_read(struct ADS101x_para_obj *ADS101x_para){
	ADS101x_para->reg_addr_ptr = ADS101x_config_reg_ptr;	//point to ADC config register
	//read config register
	ADS101x_read_reg(&(ADS101x_para->reg_addr_ptr), ADS101x_para->TRX_buff, ADS101x_reg_byte_len);
	_2char_to_16bit(ADS101x_para->TRX_buff, &(ADS101x_para->reg_val));	//convert char[2] to short for data processing
	//OR "1" to bit 16
	ADS101x_para->reg_val = ADS101x_para->reg_val | ADS101x_single_conv;	//write 1 to the MSB	
	_16bit_to_2char(ADS101x_para->TRX_buff, &(ADS101x_para->reg_val));	//convert short back to char[2] for I2C communication
	//put the data back to the config register
	ADS101x_write_reg(&(ADS101x_para->reg_addr_ptr), ADS101x_para->TRX_buff, ADS101x_reg_byte_len);

	//read ADC data
	ADS101x_para->reg_addr_ptr = ADS101x_conv_reg_ptr;	//point to ADC conversion register
	ADS101x_read_reg(&(ADS101x_para->reg_addr_ptr), ADS101x_para->TRX_buff, ADS101x_reg_byte_len);
	_2char_to_12bit(ADS101x_para->TRX_buff, &(ADS101x_para->ADC_data));	//12 bit ADC
	signed_12bit_to_signed_16bit(&(ADS101x_para->ADC_data));
}

void ADC_volt_single_conv_read(struct ADS101x_para_obj *ADS101x_para){
	ADS101x_single_conv_read(ADS101x_para);
	printf("ADC reading: %d \n", ADS101x_para->ADC_data);
	ADS101x_para->ADC_volt_read = ADS101x_para->ADC_FSR/2048 * ADS101x_para->ADC_data;
	printf("ADC Voltage = %.3fV \n", ADS101x_para->ADC_volt_read);
}


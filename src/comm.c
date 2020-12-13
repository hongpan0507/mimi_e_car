/*	***Notes***
I2C
	BCM2835 automatically pads '0' or '1' depending on I2C is a read('1') or write('0')
	It pads the direction bit to the end of the slave address, consistant to I2C protocol
	The most significant bit is shifted out first
	May need to enable I2C in Rasberry Pi system if not enabled automatically
	Need to do additional setting if trying to use I2C0; I2C1 is the default I2C channel

SPI
	Need to enable SPI manually; it is not enabled automatically by rasberry pi system
 
 */

#include <stdio.h> 
#include <stdint.h>
#include "comm.h"
#include "tmp275.h"
#include <bcm2835.h>

int DRV8343_SPI_init(){
	printf("Initialzing SPI communication...\n");
	if(!bcm2835_spi_begin()){
		printf("bcm2835_spi_begin failed. Are you running as root?\n");
		return 1;
	}	

	//SPI requirement on DRV8343 Page 52
	//DRV8343 max SPI frequency: 10MHz
	int clk_div = BCM2835_SPI_CLOCK_DIVIDER_128;
	bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST); 
	bcm2835_spi_setDataMode(BCM2835_SPI_MODE1);
	bcm2835_spi_setClockDivider(clk_div);	//3.125MHz; found in the docu, not calculated
	bcm2835_spi_chipSelect(BCM2835_SPI_CS0);
	bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS0, LOW);
	printf("SPI Clock Speed: %02x MHz \n", clk_div);
	printf("SPI communication setup completed\n"); 
	return 0;
}

// A valid data word must be 16bit long, if it's less, DRV8343 ignores the command
// DS Page52
// SPI read function is verified to work by sending IC2, IC6, IC9 address; the default content was successfully read back
int DRV8343_SPI_read(uint16_t *addr, uint16_t *read_data, bool verbose){
	//the SPI address is using 16bit address; make sure 8th bit is 1 for reading; DS Page 52
	*addr = *addr | DRV8343_SPI_read_addr_mask;
	//printf("DRV8343_SPI_address after mod: 0x%02X \n", *addr);

	/* shift is not required for some reason; it probably has something to do with how bcm2835 is sending the char out
	   shift is required when reading data out; see implementation below
	   save this line here for future reference
	//shift left address by 8 bit before sending to DRV8343; a valid word for DRV8343 must be 16bit long
	//addr = *addr << 8; 
	*/

	//bcm2835_spi_trasnfernb() is expecting char pointer for tbuff and rbuff
	// but addr and read_data are uint16_t pointers
	//explicitly convert uint16_t pointer to char pointer here; 
	//the math works out because sizeof(2 char) = sizeof(1 uint16_t); DRV8343 is expecting 2 bytes or 16bits of data
	// explicit type conversion: (char*)addr
	bcm2835_spi_transfernb((char*)addr, (char*)read_data, DRV8343_SPI_data_byte_len);	// SPI transaction
	*read_data = *read_data >> 8;	//bcm2835_spi_transfernb() flips the MISO char order; found by using logic analyzer
	*read_data = *read_data & DRV8343_SPI_read_data_mask;	//erase don't care bits
	if(verbose == true){	// good for debugging
		*addr &= ~DRV8343_SPI_read_addr_mask;	//use datasheet address for reporting
		printf("--------DRV8343 SPI Read Only-------- \n");
		printf("DRV8343_SPI_address: 0x%02X \n", *addr);
		printf("DRV8343_SPI_Read: 0x%02X \n", *read_data);
		printf("------------------------------------- \n");
	}
	return 0;
}

// A valid data word must be 16bit long, if it's less, DRV8343 ignores the command
// DS Page52
// most of the pointer type conversion is figured out by testing with SPI read; check SPI read for more documentation
int DRV8343_SPI_write(uint16_t *addr, uint16_t *write_data, uint16_t *read_data, bool verbose){
	*addr = *addr & DRV8343_SPI_write_addr_mask;	// make sure the MSB is a write for DRV8343; DS page 52

	// this is the opposite of what DRV8343 wants, but it's neccessary to do it in this order because bcm2835_spi_trasnfernb() flips the char order
	// DRV8343 wants address to go in first
	*write_data = *write_data << 8;		
	*write_data = *write_data | *addr;	//combine address and write data into one command
	bcm2835_spi_transfernb((char*)write_data, (char*)read_data, DRV8343_SPI_data_byte_len);	// SPI transaction
	*read_data = *read_data >> 8;	//bcm2835_spi_transfernb() flips the MISO char order; found by using logic analyzer
	*read_data = *read_data & DRV8343_SPI_read_data_mask;	//erase don't care bits

	if(verbose == true){
		printf("-----DRV8343 SPI Write and Read------ \n");
		printf("DRV8343_SPI_address: 0x%02X \n", *addr);
		printf("DRV8343_SPI_write: 0x%02X \n", *write_data);
		printf("DRV8343_SPI_Read: 0x%02X \n", *read_data);
		printf("------------------------------------- \n");
	}
	return 0;
}

int I2C_init(){
	printf("Initialzing I2C communication...\n");
	if(!bcm2835_i2c_begin()){
		printf("bcm2835_spi_begin failed. Are you running as root?\n");
		return 1;
	}
	int clk_div = BCM2835_I2C_CLOCK_DIVIDER_626;
	bcm2835_i2c_setClockDivider(clk_div);		//400kHz; found in the docu, not calculated
	bcm2835_i2c_setSlaveAddress(I2C1_tmp275_slave_addr);	//default slave address
	printf("I2C Clock Speed: %02X kHz \n", clk_div);
	printf("Default Slave Address is set to read TMP275 \n");
	printf("Need to set slave address before data transfer \n");
	printf("I2C communication setup completed\n");
}


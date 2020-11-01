#include <stdio.h> 
#include <stdint.h>
#include "comm.h"
#include <bcm2835.h>

int DRV8343_SPI_init(){
	printf("Initialzing SPI communication...\n");
	if(!bcm2835_spi_begin()){
		printf("bcm2835_spi_begin failed. Are you running as root?\n");
		return 1;
	}	

	//SPI requirement on DRV8343 Page 52
	//DRV8343 max SPI frequency: 10MHz
	bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST); 
	bcm2835_spi_setDataMode(BCM2835_SPI_MODE1);
	bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_128);	//3.125MHz
	bcm2835_spi_chipSelect(BCM2835_SPI_CS0);
	bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS0, LOW);
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
		printf("--------DRV8343 SPI Read Only-------- \n");
		printf("DRV8343_SPI_address: 0x%02X \n", *addr);
		printf("DRV8343_SPI_Read: 0x%02X \n", *read_data);
		printf("------------------------------------- \n");
	}
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

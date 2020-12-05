#include <stdio.h>
#include <bcm2835.h>
#include "comm.h"
#include "drv8343.h"

void DRV83xx_reg_write(uint16_t *addr, uint16_t *erase_mask, uint16_t *write_mask, uint16_t *read_data, uint16_t *write_data){
	DRV8343_SPI_read(addr, read_data, false);	// getting existing register value
	/* //for debugging only
	*read_data &= *erase_mask;				// only keep the register data that was modified 
	printf("Register Content before write: 0x%02X \n", *read_data);
	*/
	*read_data &= ~(*erase_mask);				// erase current data on the register
	*write_data = *read_data | *write_mask;					// form new register value 
	DRV8343_SPI_write(addr, write_data, read_data, false);	//write new data to the register
	DRV8343_SPI_read(addr, read_data, false);	// getting existing register value
	*read_data &= *erase_mask;				// only keep the register data that was modified 
	*addr &= DRV8343_SPI_write_addr_mask;	// set MSB to zero to make it easy on human brain
	if(*read_data != *write_mask){
		printf("***DRV83xx Register Write Error***\n");
		printf("Cannot write data to address: 0x%02X \n", *addr);
		printf("Data intended to write is: 0x%02X \n", *write_mask);
		printf("Data in the register after write command is: 0x%02X \n", *read_data);
	}else{
		printf("Register Address: 0x%02X ", *addr);
		printf("is written with: 0x%02X \n", *read_data);
	}
}

void DRV83xx_FLT_CLR(uint16_t *SPI_addr, uint16_t *write_data, uint16_t *read_data){
	//clear all latched faults
	printf("Clear All Latched Faults \n");
	*SPI_addr = SPI_REG_DRV_CTRL_1;
	DRV8343_SPI_read(SPI_addr, read_data, false);	//Read register value before  modify
	*write_data = *read_data | DRV8343S_CLR_FLT_MASK;
	DRV8343_SPI_write(SPI_addr, write_data, read_data, false);
}

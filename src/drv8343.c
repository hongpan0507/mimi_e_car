#include <stdio.h>
#include <bcm2835.h>
#include "comm.h"
#include "drv8343.h"

void DRV83xx_init(uint16_t *SPI_addr, uint16_t *write_data, uint16_t *read_data, uint16_t *erase_mask, uint16_t *write_mask){
	//clear all latched faults
	DRV83xx_FLT_CLR(SPI_addr, write_data, read_data);

	printf("set DRV8343 PWM Mode to 3x PWM Mode \n");
	*SPI_addr = SPI_REG_DRV_CTRL_1;
	*erase_mask = DRV8343S_PWM_MODE_MASK;
	*write_mask = DRV8343S_PWM_MODE_3x_MASK;
	DRV83xx_reg_write(SPI_addr, erase_mask, write_mask, read_data, write_data);

	printf("set High-side and Low-side MOSFET Drive Deadtime \n");
	*SPI_addr = SPI_REG_DRV_CTRL_9;
	*erase_mask = DRV8343S_DEAD_TIME_MASK;
	*write_mask = DRV8343S_DEAD_TIME_4000ns;
	DRV83xx_reg_write(SPI_addr, erase_mask, write_mask, read_data, write_data);

	printf("set all current sense amplifier gain = 5V/V \n");
	*SPI_addr = SPI_REG_DRV_CTRL_12;
	*erase_mask = DRV8343S_CSA_GAIN_MASK;
	*write_mask = DRV8343S_CSA_GAIN_5;
	DRV83xx_reg_write(SPI_addr, erase_mask, write_mask, read_data, write_data);

	printf("set VDS overcurrent deglitch time \n");
	*SPI_addr = SPI_REG_DRV_CTRL_10;
	*erase_mask = DRV8343S_OCP_DEG_MASK;
	//*write_mask = DRV8343S_OCP_DEG_10p25us;
	*write_mask = DRV8343S_OCP_DEG_20p5us;
	DRV83xx_reg_write(SPI_addr, erase_mask, write_mask, read_data, write_data);

	printf("set phase A VDS Fault Trip voltage\n");
	*SPI_addr = SPI_REG_DRV_CTRL_7;
	*erase_mask = DRV8343S_VDS_LVL_LA_MASK;
	//*write_mask = DRV8343S_VDS_LVL_LA_0p06V;
	//*write_mask = DRV8343S_VDS_LVL_LA_0p13V;
	//*write_mask = DRV8343S_VDS_LVL_LA_0p20V;
	*write_mask = DRV8343S_VDS_LVL_LA_0p26V;
	//*write_mask = DRV8343S_VDS_LVL_LA_0p31V;
	DRV83xx_reg_write(SPI_addr, erase_mask, write_mask, read_data, write_data);
	*erase_mask = DRV8343S_VDS_LVL_HA_MASK;
	//*write_mask = DRV8343S_VDS_LVL_HA_0p06V;
	//*write_mask = DRV8343S_VDS_LVL_HA_0p13V;
	//*write_mask = DRV8343S_VDS_LVL_HA_0p20V;
	*write_mask = DRV8343S_VDS_LVL_HA_0p26V;
	//*write_mask = DRV8343S_VDS_LVL_HA_0p31V;
	DRV83xx_reg_write(SPI_addr, erase_mask, write_mask, read_data, write_data);
	DRV8343_SPI_read(SPI_addr, read_data, true);

	printf("set phase B VDS Fault Trip voltage\n");
	*SPI_addr = SPI_REG_DRV_CTRL_7;
	*erase_mask = DRV8343S_VDS_LVL_LB_MASK;
	//*write_mask = DRV8343S_VDS_LVL_LB_0p06V;
	//*write_mask = DRV8343S_VDS_LVL_LB_0p13V;
	//*write_mask = DRV8343S_VDS_LVL_LB_0p20V;
	*write_mask = DRV8343S_VDS_LVL_LB_0p26V;
	//*write_mask = DRV8343S_VDS_LVL_LB_0p31V;
	DRV83xx_reg_write(SPI_addr, erase_mask, write_mask, read_data, write_data);
	*erase_mask = DRV8343S_VDS_LVL_HB_MASK;
	//*write_mask = DRV8343S_VDS_LVL_HB_0p06V;
	//*write_mask = DRV8343S_VDS_LVL_HB_0p13V;
	//*write_mask = DRV8343S_VDS_LVL_HB_0p20V;
	*write_mask = DRV8343S_VDS_LVL_HB_0p26V;
	//*write_mask = DRV8343S_VDS_LVL_HB_0p31V;
	DRV83xx_reg_write(SPI_addr, erase_mask, write_mask, read_data, write_data);
	DRV8343_SPI_read(SPI_addr, read_data, true);

	printf("set overcurrent protection mode\n");
	*SPI_addr = SPI_REG_DRV_CTRL_11;
	*erase_mask = DRV8343S_OCP_MODE_MASK;
	//*write_mask = DRV8343S_OCP_MODE_OFF;
	*write_mask = DRV8343S_OCP_MODE_LATCHED_FLT;
	//*write_mask = DRV8343S_OCP_MODE_RETRY;
	DRV83xx_reg_write(SPI_addr, erase_mask, write_mask, read_data, write_data);

	printf("set Fault Retry Time\n");
	*SPI_addr = SPI_REG_DRV_CTRL_9;
	*erase_mask = DRV8343S_TRETRY_MASK;
	*write_mask = DRV8343S_TRETRY_8ms;
	DRV83xx_reg_write(SPI_addr, erase_mask, write_mask, read_data, write_data);

	printf("disable current sense amplifier overcurrent fault detection\n");
	*SPI_addr = SPI_REG_DRV_CTRL_14;
	*erase_mask = DRV8343S_DIS_SEN_ABC_MASK;
	*write_mask = DRV8343S_DIS_SEN_ABC;
	DRV83xx_reg_write(SPI_addr, erase_mask, write_mask, read_data, write_data);
}

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

#include <bcm2835.h>
#include <stdio.h>
#include "e_car_ctrl.h"
#include "comm.h"
#include "drv8343.h"	// extract from DRV8343 eval board software program

/* Notes:
   1. GPIO 13 cannot used as input for reading; bad pin? or bcm2835 library problem?
*/


int main(int argc, char **argv){
	/*
	uint16_t test = 0xF000;
	printf("before bitwise shift left: 0x%04X \n", test);
	test = test << 2;
	printf("after bitwise shift left: 0x%04X \n", test);
	*/
	if(!bcm2835_init()){
		printf("Failed to initialize BCM2835 library");
		return 1;
	}

	// the order of the initialization routine matters because bcm2835 SPI library use
	//pin24/GPIO8 and pin26/GPIO7 as SS
	//need to set one of the SS pins as output again if want to use it as output
	//in e_car_init(), pin26/GPIO7 is used as an output to enable/disable DRV8343 gate driver
	// e_car_inint() needs to be placed after DRV8343_SPI_init();
	DRV8343_SPI_init();
	e_car_init();	//set up raspberry pi pin paramemeter

	uint16_t SPI_addr = 0;
	uint16_t write_data = 0;	//set DRV8343 PWM Mode to 3x PWM Mode
	uint16_t read_data = 0;

	//set DRV8343 PWM Mode to 3x PWM Mode
	printf("set DRV8343 PWM Mode to 3x PWM Mode \n");
	SPI_addr = SPI_REG_DRV_CTRL_1;
	write_data = 0b00010000;	
	DRV8343_SPI_write(&SPI_addr, &write_data, &read_data);
	//DRV8343_SPI_read(&SPI_addr, &read_data);	//used for initial SPI communication testing

	//set all current sense amplifier gain = 5V/V
	printf("set all current sense amplifier gain = 5V/V \n");
	SPI_addr = SPI_REG_DRV_CTRL_12;
	write_data = 0b00000000;	
	DRV8343_SPI_write(&SPI_addr, &write_data, &read_data);

	//clear all latched faults
	printf("Clear Latched Faults\n");
	SPI_addr = SPI_REG_DRV_CTRL_1;
	DRV8343_SPI_read(&SPI_addr, &read_data);	//Read register value before  modify
	write_data = read_data | DRV8343S_CLR_FLT_MASK;
	DRV8343_SPI_write(&SPI_addr, &write_data, &read_data);

	int pwm_data = 0;
	uint8_t Motor_DIR_val = 0;
	uint8_t pre_PEDAL_val = 0;
	uint8_t PEDAL_val = 0;
	uint8_t DRV8343_FLT_val = 0;

	while(1){
		DRV8343_FLT_val = bcm2835_gpio_lev(DRV8343_FLT);	//read DRV8343 general Fault status
		bcm2835_pwm_set_data(PWM_CHANNEL0, 512);
		delay(PWM_delay);	//delay in ms

		if(DRV8343_FLT_val == 0){	//active low
			printf("DRV8343 fault flag is active; Check Fault Status and DIAG Status Register for more details \n");
			printf("SPI_REG_FAULT_STAT \n");
			SPI_addr = SPI_REG_FAULT_STAT;
			DRV8343_SPI_read(&SPI_addr, &read_data);
			printf("SPI_REG_DIAG_STAT_A \n");
			SPI_addr = SPI_REG_DIAG_STAT_A;
			DRV8343_SPI_read(&SPI_addr, &read_data);
			printf("SPI_REG_DIAG_STAT_B \n");
			SPI_addr = SPI_REG_DIAG_STAT_B;
			DRV8343_SPI_read(&SPI_addr, &read_data);
			printf("SPI_REG_DIAG_STAT_C \n");
			SPI_addr = SPI_REG_DIAG_STAT_C;
			DRV8343_SPI_read(&SPI_addr, &read_data);
			return 0;
		}
	}
	while(1){
		Motor_DIR_val = bcm2835_gpio_lev(Motor_DIR_PIN_IN);	//reading switch state
		if(Motor_DIR_val == 0)
			;
			//bcm2835_gpio_write(Motor_DIR_PIN_OUT, HIGH);	//change direction
		else
			printf("");;
			//bcm2835_gpio_write(Motor_DIR_PIN_OUT, LOW);	//change direction
		printf("Motor_DIR_val = %d\n", Motor_DIR_val);
		
		PEDAL_val = bcm2835_gpio_lev(PEDAL_PIN); //reading Pedal state
		if(PEDAL_val == 1){		//Pedal press detected; speed up
			if(PEDAL_val != pre_PEDAL_val){	//start from init_speed instead of 0
				pwm_data = init_speed;	
			}
			if(pwm_data < max_speed - 1){	//check for max speed
				pwm_data += PWM_DCIR;	//increase duty cycle
			}
			bcm2835_pwm_set_data(PWM_CHANNEL0, pwm_data);	//write duty cycle
		} else if(PEDAL_val == 0){	//Pedal is released; slow down
			if(pwm_data > init_speed){	//check for initial speed
				pwm_data -= PWM_DCIR*SS_ratio;	//decrease duty cycle
			} else {
				pwm_data = 0;	//stop motor completely
			}
			bcm2835_pwm_set_data(PWM_CHANNEL0, pwm_data);	//write duty cycle
		} else {	//just in case something goes wrong; turn off motor
			bcm2835_pwm_set_data(PWM_CHANNEL0, 0);
		}
		pre_PEDAL_val = PEDAL_val;	//record previous Pedal state
		printf("pwm_data = %d\n", pwm_data);
		printf("Pedal val = %d\n", PEDAL_val);

		delay(PWM_delay);	//delay in ms
	}

	bcm2835_close();
	return 0;
}


#include <bcm2835.h>
#include <stdio.h>
#include "e_car_ctrl.h"
#include "comm.h"
#include "drv8343.h"	// extract from DRV8343 eval board software program
#include "tmp275.h"
#include "ADS101x.h"
#include "utilities.h"
#include <math.h>

/* Notes:
   1. GPIO 13 cannot used as input for reading; bad pin? or bcm2835 library problem?
*/


int main(int argc, char **argv){

	if(!bcm2835_init()){
		printf("Failed to initialize BCM2835 library");
		return 1;
	}

	//erase_mask is the same as the location in a register where the data is written to
	//write_mask is the data to be written to a register
	//xxx_mask convetion comes from naming convetion in TI drv8343.h
	uint16_t SPI_addr = 0;
	uint16_t write_data = 0;	
	uint16_t read_data = 0;
	uint16_t erase_mask = 0;
	uint16_t write_mask = 0;

	short PCB_tmp_warm_high_limit = 35;	//if PCB temperature goes over this number; turn on the fan
	short PCB_tmp_warm_low_limit = 32;	//if PCB temperature goes over this number; turn on the fan
	//short tmp_DAC_data = 0; 
	extern float PCB_tmp_C;		// PCB temperature data read from TMP275 closest to the MOSFETs
	//float PCB_tmp_F = 0;		// PCB temperature data read from TMP275 closest to the MOSFETs

	// the order of the initialization routine matters because bcm2835 SPI library use
	//pin24/GPIO8 and pin26/GPIO7 as SS
	//need to set one of the SS pins as output again if want to use it as output
	//in e_car_init(), pin26/GPIO7 is used as an output to enable/disable DRV8343 gate driver
	// e_car_inint() needs to be placed after DRV8343_SPI_init();
	DRV8343_SPI_init();
	DRV83xx_init(&SPI_addr, &write_data, &read_data, &erase_mask, &write_mask);
	e_car_init();	//set up raspberry pi pin paramemeter

	uint16_t PWM_val = 0;
	uint32_t time_count = 0;		// for PWM control 
	//ramp_rate was found by tuning
	//initial value is 0.00001
	//time_count%100000 was used
	float ramp_up_rate = PWM_ramp_up_abs_min;		//controlled by acce knob
	float ramp_down_rate = PWM_ramp_down_abs_min;	//not controlled
	extern float acce_val_read;	
	uint16_t init_PWM_val = PWM_init;
	
	int pwm_data = 0;
	uint8_t Motor_brake_val = 0;
	uint8_t Motor_DIR_val = 0;
	uint8_t pre_PEDAL_val = 0;
	uint8_t PEDAL_val = 0;
	uint8_t DRV8343_FLT_val = 0;
	uint8_t DRV8343_FLT_report = 0;
	uint8_t Motor_DRV_FLT_RST_val = 0;
	uint8_t Motor_DRV_FLT_RST_count = 0;
	int RETRY_count = 0;
	uint8_t over_tmp_FLT = 0;
	
	uint32_t tmp_loop_count = 0;		// for reporting temperature
	uint32_t tmp_report_count = 0;		// for reporting temperature
	uint32_t speed_loop_count = 0;		// for reporting temperature
	uint32_t speed_knob_report_count = 0;		// for reporting temperature
	tmp_report_count = pow(2, 22);		// report about once every seconds 
	speed_knob_report_count = tmp_report_count/50;	//found by trial and error 

	//Only use for debugging when stop and start motor control is required
//	char c;
//	printf("Enter any character to continue:");
//	c = getchar();
//	printf("Character entered: ");
//	putchar(c);
	printf("Ready to Roll \n");

	while(1){
		//Sensor reading
		DRV8343_FLT_val = bcm2835_gpio_lev(DRV8343_FLT);	//read DRV8343 general Fault status
		over_tmp_FLT = bcm2835_gpio_lev(Power_MOSFET_over_tmp_alert);	//read TMP275 temperature; active high

		//Motor control
		PEDAL_val = bcm2835_gpio_lev(PEDAL_PIN); //reading Pedal state
		Motor_brake_val = bcm2835_gpio_lev(Motor_BRAKE_PIN); //reading Pedal state
		if(Motor_brake_val == 1){	//control by brake switch
			if(PWM_val == 0){	//only allows change of direction when PWM is zero
				//Motor_DIR_val: 1 = forward; 0 = backward
				Motor_DIR_val = bcm2835_gpio_lev(Motor_DIR_PIN_IN);	//reading switch state
				//printf("PEDAL_val: %d \n", PEDAL_val);
				//printf("Motor_DIR_val: %d \n", Motor_DIR_val);
			}
			if(PEDAL_val == 1){		//Pedal pressed
				motor_gentle_start(&PWM_val, &time_count, &ramp_up_rate, &init_PWM_val, &Motor_DIR_val);
			}else if(PEDAL_val == 0){	//Pedal released
				motor_gentle_stop(&PWM_val, &time_count, &ramp_up_rate, &init_PWM_val, &Motor_DIR_val);
			}
		}else{		//control by brake switch
			//motor_coast(coast_ON);
			//motor_brake();
			//brake the motor when pwm is slowed down to zero
			if(PWM_val == 0){
				motor_brake();
			}else{
				motor_gentle_stop(&PWM_val, &time_count, &ramp_down_rate, &init_PWM_val, &Motor_DIR_val);
			}
		}

		// Fault Handling	
		while(DRV8343_FLT_val == 0){	//active low
			motor_coast(coast_ON);
			motor_PWM_reset();
			PWM_val = 0;
			time_count = 0;
			if(DRV8343_FLT_report == 0){	// only report once for each fault condition; then wait for reset signal
				printf("DRV8343 fault flag is active; Check Fault Status and DIAG Status Register for more details \n");
				printf("SPI_REG_FAULT_STAT \n");
				SPI_addr = SPI_REG_FAULT_STAT;
				DRV8343_SPI_read(&SPI_addr, &read_data, true);
				printf("SPI_REG_DIAG_STAT_A \n");
				SPI_addr = SPI_REG_DIAG_STAT_A;
				DRV8343_SPI_read(&SPI_addr, &read_data, true);
				printf("SPI_REG_DIAG_STAT_B \n");
				SPI_addr = SPI_REG_DIAG_STAT_B;
				DRV8343_SPI_read(&SPI_addr, &read_data, true);
				printf("SPI_REG_DIAG_STAT_C \n");
				SPI_addr = SPI_REG_DIAG_STAT_C;
				DRV8343_SPI_read(&SPI_addr, &read_data, true);
				++RETRY_count;
				printf("Retry count: %d \n\n", RETRY_count);
			}
			//!!!!!!!!!turn on the status warning light here!!!!!!
			Motor_DRV_FLT_RST_val = bcm2835_gpio_lev(Motor_DRV_FLT_RST_PIN);	//read the status of driver fault reset pin
			if(Motor_DRV_FLT_RST_val == 0){
				++Motor_DRV_FLT_RST_count;
				if(Motor_DRV_FLT_RST_count >= 20){	//press reset button for xx of the delay(100ms)
					DRV83xx_FLT_CLR(&SPI_addr, &write_data, &read_data);
					printf("\n\n DRV8343 Fault Cleared \n\n");
					motor_brake();
					DRV8343_FLT_report = 0;
					Motor_DRV_FLT_RST_count = 0;
					break;
				}
			}
			DRV8343_FLT_report = 1;
			delay(100);
		}
		if(over_tmp_FLT == 1){	//active high; overheating
			//Power MOSFET over temperature alert
			//alert will stop according to the temperature setting in TMP275.h
			power_MOSFET_cooling_fan_CTRL(fan_ON);	// turn on cooling fan and LED indicator
			while(over_tmp_FLT == 1){	// as long as PCB is overheating
				printf("PCB overheating!!! \n");
				motor_coast(coast_ON);		//slow down to stop
				//power_MOSFET_TMP_report(&PCB_tmp_C, &PCB_tmp_F, &tmp_DAC_data);
				power_MOSFET_TMP_report();
				delay(1000);	// report once a second
				over_tmp_FLT = bcm2835_gpio_lev(Power_MOSFET_over_tmp_alert);	//read TMP275 temperature; active high
			}
			motor_coast(coast_OFF);		//Enable PWM
			power_MOSFET_cooling_fan_CTRL(fan_OFF);	// turn off cooling fan after PCB cools down
		}

		// Reporting
		if(speed_loop_count >	speed_knob_report_count){		// depends loop execution delay
			speed_ctrl_knob_read();
			acce_ctrl_knob_read();
			ramp_up_rate = acce_val_read;
			speed_loop_count = 0;
		}
		if(tmp_loop_count >	tmp_report_count){	// depends loop execution delay
			//printf("tmp_loop_count=%d \n", tmp_loop_count);
			power_MOSFET_TMP_report();
			//PCB warming up to the upper limit; cool it down
			if(PCB_tmp_C > PCB_tmp_warm_high_limit){
				power_MOSFET_cooling_fan_CTRL(fan_ON);	
			} else if(PCB_tmp_C < PCB_tmp_warm_low_limit){	// turn off 
				power_MOSFET_cooling_fan_CTRL(fan_OFF);	
			}
			tmp_loop_count = 0;
		}
		++tmp_loop_count;
		++speed_loop_count;
	}

	bcm2835_close();
	return 0;
}


#include <bcm2835.h>
#include <stdio.h>
#include "e_car_ctrl.h"
#include "comm.h"
#include "drv8343.h"	// extract from DRV8343 eval board software program
#include "tmp275.h"
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

	// the order of the initialization routine matters because bcm2835 SPI library use
	//pin24/GPIO8 and pin26/GPIO7 as SS
	//need to set one of the SS pins as output again if want to use it as output
	//in e_car_init(), pin26/GPIO7 is used as an output to enable/disable DRV8343 gate driver
	// e_car_inint() needs to be placed after DRV8343_SPI_init();
	DRV8343_SPI_init();
	I2C_init();
	tmp275_init();	//temperature sensor init
	e_car_init();	//set up raspberry pi pin paramemeter

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
	short tmp_DAC_data = 0; 
	float PCB_tmp_C = 0;		// PCB temperature data read from TMP275 closest to the MOSFETs
	float PCB_tmp_F = 0;		// PCB temperature data read from TMP275 closest to the MOSFETs

	//clear all latched faults
	DRV83xx_FLT_CLR(&SPI_addr, &write_data, &read_data);

	printf("set DRV8343 PWM Mode to 3x PWM Mode \n");
	SPI_addr = SPI_REG_DRV_CTRL_1;
	erase_mask = DRV8343S_PWM_MODE_MASK;
	write_mask = DRV8343S_PWM_MODE_3x_MASK;
	DRV83xx_reg_write(&SPI_addr, &erase_mask, &write_mask, &read_data, &write_data);

	printf("set High-side and Low-side MOSFET Drive Deadtime \n");
	SPI_addr = SPI_REG_DRV_CTRL_9;
	erase_mask = DRV8343S_DEAD_TIME_MASK;
	write_mask = DRV8343S_DEAD_TIME_4000ns;
	DRV83xx_reg_write(&SPI_addr, &erase_mask, &write_mask, &read_data, &write_data);

	printf("set all current sense amplifier gain = 5V/V \n");
	SPI_addr = SPI_REG_DRV_CTRL_12;
	erase_mask = DRV8343S_CSA_GAIN_MASK;
	write_mask = DRV8343S_CSA_GAIN_5;
	DRV83xx_reg_write(&SPI_addr, &erase_mask, &write_mask, &read_data, &write_data);

	printf("set VDS overcurrent deglitch time \n");
	SPI_addr = SPI_REG_DRV_CTRL_10;
	erase_mask = DRV8343S_OCP_DEG_MASK;
	write_mask = DRV8343S_OCP_DEG_20p5us;
	DRV83xx_reg_write(&SPI_addr, &erase_mask, &write_mask, &read_data, &write_data);

	printf("set phase B VDS Fault Trip voltage\n");
	SPI_addr = SPI_REG_DRV_CTRL_7;
	erase_mask = DRV8343S_VDS_LVL_LB_MASK;
	write_mask = DRV8343S_VDS_LVL_LB_1p88V;
	DRV83xx_reg_write(&SPI_addr, &erase_mask, &write_mask, &read_data, &write_data);
	erase_mask = DRV8343S_VDS_LVL_HB_MASK;
	write_mask = DRV8343S_VDS_LVL_HB_1p88V;
	DRV83xx_reg_write(&SPI_addr, &erase_mask, &write_mask, &read_data, &write_data);
	DRV8343_SPI_read(&SPI_addr, &read_data, true);

	printf("set overcurrent protection mode\n");
	SPI_addr = SPI_REG_DRV_CTRL_11;
	erase_mask = DRV8343S_OCP_MODE_MASK;
	//write_mask = DRV8343S_OCP_MODE_OFF;
	//write_mask = DRV8343S_OCP_MODE_LATCHED_FLT;
	write_mask = DRV8343S_OCP_MODE_RETRY;
	DRV83xx_reg_write(&SPI_addr, &erase_mask, &write_mask, &read_data, &write_data);

	printf("set Fault Retry Time\n");
	SPI_addr = SPI_REG_DRV_CTRL_9;
	erase_mask = DRV8343S_TRETRY_MASK;
	write_mask = DRV8343S_TRETRY_2ms;
	DRV83xx_reg_write(&SPI_addr, &erase_mask, &write_mask, &read_data, &write_data);

	printf("disable current sense amplifier overcurrent fault detection\n");
	SPI_addr = SPI_REG_DRV_CTRL_14;
	erase_mask = DRV8343S_DIS_SEN_ABC_MASK;
	write_mask = DRV8343S_DIS_SEN_ABC;
	DRV83xx_reg_write(&SPI_addr, &erase_mask, &write_mask, &read_data, &write_data);


	
	uint16_t PWM_val = 0;
	uint32_t time_count = 0;		// for PWM control 
	//ramp_rate was found by tuning
	//initial value is 0.00001
	//time_count%100000 was used
	float ramp_rate = PWM_ramp_rate;
	
	uint16_t init_PWM_val = PWM_init;
	
	int pwm_data = 0;
	uint8_t Motor_coast_val = 0;
	uint8_t Motor_DIR_val = 0;
	uint8_t pre_PEDAL_val = 0;
	uint8_t PEDAL_val = 0;
	uint8_t DRV8343_FLT_val = 0;
	uint8_t Motor_DRV_FLT_RST_val = 0;
	int RETRY_count = 0;
	uint8_t over_tmp_FLT = 0;
	
	uint32_t loop_count = 0;		// for reporting temperature
	uint32_t report_count = 0;		// for reporting temperature
	report_count = pow(2, 22);		// report about once every seconds 
	
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
		Motor_coast_val = bcm2835_gpio_lev(Motor_COAST_PIN); //reading Pedal state
		if(Motor_coast_val == 0){	//control by coast switch
			if(PWM_val == 0){	//only allows change of direction when PWM is zero
				//Motor_DIR_val: 1 = forward; 0 = backward
				Motor_DIR_val = bcm2835_gpio_lev(Motor_DIR_PIN_IN);	//reading switch state
				//printf("PEDAL_val: %d \n", PEDAL_val);
				//printf("Motor_DIR_val: %d \n", Motor_DIR_val);
			}
			if(PEDAL_val == 1){		//Pedal pressed
				motor_coast(coast_OFF);
				if(PWM_val < PWM_max){	//keep increase PWM until reaching max
					time_count++;		//allow time to count up
					//only update PWM value after some time has passed
					//highly dependant on how fast the program is going through the loop
					//ideally timer interrupt should be used here
					if(time_count%PWM_time_unit){	
						motor_speed_ctrl_linear(&PWM_val, &ramp_rate, &init_PWM_val, &time_count);
						//printf("PWM_val: %d\n", PWM_val);
					}
				}
			}else if(PEDAL_val == 0){	//Pedal released
				motor_gentle_stop(&PWM_val, &time_count, &ramp_rate, &init_PWM_val);
			}
			motor_move(&PWM_val, &Motor_DIR_val);
			pre_PEDAL_val = PEDAL_val;	//record previous Pedal state
		}else{		//control by coast switch
//			motor_coast(coast_ON);
//			PWM_val = 0;
//			time_count = 0;
			motor_gentle_stop(&PWM_val, &time_count, &ramp_rate, &init_PWM_val);
		}

		// Fault Handling	
		if(DRV8343_FLT_val == 0){	//active low
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
			//return 0;
		}
		if(over_tmp_FLT == 1){	//active high; overheating
			//Power MOSFET over temperature alert
			//alert will stop according to the temperature setting in TMP275.h
			power_MOSFET_cooling_fan_CTRL(fan_ON);	// turn on cooling fan and LED indicator
			while(over_tmp_FLT == 1){	// as long as PCB is overheating
				printf("PCB overheating!!! \n");
				motor_coast(coast_ON);		//slow down to stop
				tmp275_tmp_report(&PCB_tmp_C, &PCB_tmp_F, &tmp_DAC_data);
				delay(1000);	// report once a second
				over_tmp_FLT = bcm2835_gpio_lev(Power_MOSFET_over_tmp_alert);	//read TMP275 temperature; active high
			}
			motor_coast(coast_OFF);		//Enable PWM
			power_MOSFET_cooling_fan_CTRL(fan_OFF);	// turn off cooling fan after PCB cools down
		}

		// Reporting
		if(loop_count >	report_count){	// depends loop execution delay
			//printf("loop_count=%d \n", loop_count);
			//tmp275_tmp_report(&PCB_tmp_C, &PCB_tmp_F, &tmp_DAC_data);
			//PCB warming up to the upper limit; cool it down
			if(PCB_tmp_C > PCB_tmp_warm_high_limit){
				power_MOSFET_cooling_fan_CTRL(fan_ON);	
			} else if(PCB_tmp_C < PCB_tmp_warm_low_limit){	// turn off 
				power_MOSFET_cooling_fan_CTRL(fan_OFF);	
			}
			loop_count = 0;
		}
		loop_count++;
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


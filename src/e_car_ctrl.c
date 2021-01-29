#include <stdio.h>
#include "e_car_ctrl.h"
#include "tmp275.h"
#include "comm.h"
#include "ADS101x.h"
#include <bcm2835.h>

uint16_t PWM_val_read = 0;
float acce_val_read = 0;

int e_car_init(){
	printf("Initializing e_car parameters...\n");
	//set up I2C communication
	I2C_init();
	tmp275_init();	//temperature sensor init
	ADS101x_init();	//set up ADC 
	
	//Set GPIO pin as output
	bcm2835_gpio_fsel(PWM_INHC, BCM2835_GPIO_FSEL_OUTP);	//Use PWM_INHC as GPIO to control cooling fan
	bcm2835_gpio_fsel(DRV8343_EN, BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_fsel(PWM_INLx_EN, BCM2835_GPIO_FSEL_OUTP);
	printf("Enabling DRV8343 Gate Driver \n");	
	bcm2835_gpio_write(DRV8343_EN, HIGH);	//Enable DRV8343 Gate Driver
	//Enable PWM in 3xPMW Mode; check DS P21 for more detail 
	//Motor Coasting disabled; ready to receive PWM
	bcm2835_gpio_write(PWM_INLx_EN, HIGH);	

	//set GPIO pin as input and set up pull up or pull down
	bcm2835_gpio_fsel(Motor_BRAKE_PIN, BCM2835_GPIO_FSEL_INPT);
	bcm2835_gpio_set_pud(Motor_BRAKE_PIN, BCM2835_GPIO_PUD_UP);	//pull up
	bcm2835_gpio_fsel(Motor_DIR_PIN_IN, BCM2835_GPIO_FSEL_INPT);
	bcm2835_gpio_set_pud(Motor_DIR_PIN_IN, BCM2835_GPIO_PUD_UP);	//pull up
	bcm2835_gpio_fsel(PEDAL_PIN, BCM2835_GPIO_FSEL_INPT);
	bcm2835_gpio_set_pud(PEDAL_PIN, BCM2835_GPIO_PUD_UP);			//pull up
	bcm2835_gpio_fsel(DRV8343_FLT, BCM2835_GPIO_FSEL_INPT);
	//cannot use pull up here; input is at 1.2V when Fault is active (low)
	//there is a 10k pull-up on the board
	//it looks like DRV8343 open drain pin does not have enough strength to pull the line low when Raspberry Pi pull-up is enabled
	//The downside of this is that this programm will keep reporting DRV8343 Fault Flag when the line is not connected
	bcm2835_gpio_set_pud(DRV8343_FLT, BCM2835_GPIO_PUD_DOWN);	//pull down
	bcm2835_gpio_fsel(Motor_DRV_FLT_RST_PIN, BCM2835_GPIO_FSEL_INPT);
	bcm2835_gpio_set_pud(Motor_DRV_FLT_RST_PIN, BCM2835_GPIO_PUD_UP);	//pull up

	//set GPIO pin as input for TMP275 alert
	// Pull up is already installed on PCB
	// active high
	bcm2835_gpio_fsel(Power_MOSFET_over_tmp_alert, BCM2835_GPIO_FSEL_INPT);

	//Set the GPIO pin to the correct Alt Fun in order to use PWM channel 0 at the pin
	bcm2835_gpio_fsel(PWM_INHA, BCM2835_GPIO_FSEL_ALT0);
	bcm2835_gpio_fsel(PWM_INHB, BCM2835_GPIO_FSEL_ALT0);

	//Set clock divider to 16
	//Set range to 1024 in Markspace Mode
	//PRF = (19.2MHz/CLK_divider/PWM Range = PWM freq in Hz
	//PRF=(19.2MHz/16)/1024=1171.875Hz
	//PRF=(19.2MHz/2)/1024=9375Hz
	//PRF=(19.2MHz/2)/512=18.75kHz
	printf("Setting PWM Parameter \n");
	bcm2835_pwm_set_clock(BCM2835_PWM_CLOCK_DIVIDER_2);
	//bcm2835_pwm_set_clock(BCM2835_PWM_CLOCK_DIVIDER_4);
	//bcm2835_pwm_set_clock(BCM2835_PWM_CLOCK_DIVIDER_8);
	//bcm2835_pwm_set_clock(BCM2835_PWM_CLOCK_DIVIDER_16);
	bcm2835_pwm_set_mode(PWM_CHANNEL0, 1, 1);
	bcm2835_pwm_set_range(PWM_CHANNEL0, PWM_RANGE);
	bcm2835_pwm_set_mode(PWM_CHANNEL1, 1, 1);
	bcm2835_pwm_set_range(PWM_CHANNEL1, PWM_RANGE);

	//start up initial condition
	motor_PWM_reset();
	motor_coast(coast_ON);
	power_MOSFET_cooling_fan_CTRL(fan_OFF);
	
//	while(1){
//		speed_ctrl_knob_read();
//		power_MOSFET_TMP_report();
//		delay(100);
//	}

	printf("e_car Initialization Completed!\n");
}

//reset all of the PWM value to zero
void motor_PWM_reset(){
	bcm2835_pwm_set_data(PWM_CHANNEL0, 0);	
	bcm2835_pwm_set_data(PWM_CHANNEL1, 0);
}

void motor_DIR(){
	int Motor_DIR_val = bcm2835_gpio_lev(Motor_DIR_PIN_IN);	//reading switch state
	/*
		if(Motor_DIR_val == 0)
			//bcm2835_gpio_write(Motor_DIR_PIN_OUT, HIGH);	//change direction
			;
		else
			//bcm2835_gpio_write(Motor_DIR_PIN_OUT, LOW);	//change direction
		printf("Motor_DIR_val = %d\n", Motor_DIR_val);
		*/

}

// linear ramp up or down
// y=m*x+b
// y=PWM; m=ramp rate; x=time(count); b=initial PWM
// ideally time should be better controlled, like using timer interrupt
// with Raspi, there might be a way to use timer interrupt in the future
// for now time is controlled by the time it takes to execute the code in the while loop
// the time is tracked by counting the loop
void motor_speed_ctrl_linear(uint16_t *PWM_val, float *ramp_rate, uint16_t *init_PWM_val,  uint32_t *time_count){
	//time_count is large; normalize it and use ramp_rate to control speed
	//ideally ramp_rate = 1 because PWM_val is integer; float number will convert to the same integer before it's big enough
	//keep increasing PWM_time_unit if ramp_rate is not close to 1
	*PWM_val = *ramp_rate * *time_count/PWM_time_unit + *init_PWM_val;
//	if(*PWM_val > PWM_abs_max){
//		*PWM_val = PWM_abs_max;
//	}else if(*PWM_val < *init_PWM_val){
//		*PWM_val = *init_PWM_val;
//	}
	//printf("PWM_val= %d \n", *PWM_val);
}

void motor_move(uint16_t *PWM_val, uint8_t *motor_DIR_val){
	if(*motor_DIR_val == 1){	// move forward
		bcm2835_pwm_set_data(PWM_CHANNEL0, *PWM_val);	//change motor speed 
		bcm2835_pwm_set_data(PWM_CHANNEL1, 0);	//change motor speed 
	} else if (*motor_DIR_val == 0){	//move backward
		bcm2835_pwm_set_data(PWM_CHANNEL0, 0);	//change motor speed 
		bcm2835_pwm_set_data(PWM_CHANNEL1, *PWM_val);	//change motor speed 
	}
}

// Synchronus delay
// current slow decay; Back EMF fast collapse
// shorting both ends of the terminal to the ground return causes RasPi to restart
// probably because sudden change in Di/Dt raises ground voltage too high
// shorting both end of the motor terminal to the power rail
// may be putting the gate driver into Hi-Z state will help
// check DS page 21 for more detail
void motor_brake(){
	motor_coast(coast_OFF);
	bcm2835_pwm_set_data(PWM_CHANNEL0, 0);	// turn off motor
	bcm2835_pwm_set_data(PWM_CHANNEL1, 0);	// turn off motor
}

// current fast decay
// Place all the Power MOSFET into Hi Z mode
// Motor Current circulates through MOSFET body diode
// Check DS 21 for more detail
void motor_coast(uint8_t CTRL){
	if(CTRL == 1){
		bcm2835_gpio_write(PWM_INLx_EN, LOW);	
	} else if (CTRL == 0){
		bcm2835_gpio_write(PWM_INLx_EN, HIGH);	
	}
}

void motor_gentle_start(uint16_t *PWM_val, uint32_t *time_count, float *ramp_rate, uint16_t *init_PWM_val, uint8_t *Motor_DIR_val){
	motor_coast(coast_OFF);
	if(*PWM_val < PWM_val_read){	//keep increase PWM until reaching max defineb by PWM_val_read 
		//time_count++;		//allow time to count up
 		*time_count = *time_count + 1;		//allow time to count up
		//only update PWM value after some time has passed
		//highly dependant on how fast the program is going through the loop
		//ideally timer interrupt should be used here
		if(*time_count%PWM_time_unit){	
 			motor_speed_ctrl_linear(PWM_val, ramp_rate, init_PWM_val, time_count);
			//printf("PWM_val: %d\n", PWM_val);
		}
	}else if(*PWM_val > PWM_val_read){	//slow down
 		*time_count = *time_count - 1;		//allow time to count up
		//only update PWM value after some time has passed
		//highly dependant on how fast the program is going through the loop
		//ideally timer interrupt should be used here
		if(*time_count%PWM_time_unit){	
 			motor_speed_ctrl_linear(PWM_val, ramp_rate, init_PWM_val, time_count);
			//printf("PWM_val: %d\n", PWM_val);
		}
	}
	motor_move(PWM_val, Motor_DIR_val);
}

void motor_gentle_stop(uint16_t *PWM_val, uint32_t *time_count, float *ramp_rate, uint16_t *init_PWM_val, uint8_t *Motor_DIR_val){
 	if(*PWM_val > *init_PWM_val){
 		//*time_count--;		//allow time to count up
 		*time_count = *time_count - 1;		//allow time to count up
 		//only update PWM value after some time has passed
 		//highly dependant on how fast the program is going through the loop
 		//ideally timer interrupt should be used here
 		if(*time_count%PWM_time_unit){	
 			motor_speed_ctrl_linear(PWM_val, ramp_rate, init_PWM_val, time_count);
 			//printf("PWM_val: %d\n", PWM_val);
 		}
		motor_move(PWM_val, Motor_DIR_val);
 	}else{
 		motor_coast(coast_ON);
		motor_PWM_reset();
 		*PWM_val = 0;
 		*time_count = 0;
 	}
}

void power_MOSFET_cooling_fan_CTRL(uint8_t CTRL){
	if(CTRL == 0){	
		bcm2835_gpio_write(PWM_INHC, LOW);	//turn off fan
	} else if(CTRL == 1){	//turn on fan
		bcm2835_gpio_write(PWM_INHC, HIGH);	//turn on fan
	}
}

void power_MOSFET_TMP_report(){
	extern uint16_t I2C_addr;
	I2C_addr = I2C1_tmp275_slave_addr;
	I2C_set_addr(&I2C_addr);

	extern float PCB_tmp_C;
	extern float PCB_tmp_F;
	extern short tmp_DAC_data;
	tmp275_tmp_report(&PCB_tmp_C, &PCB_tmp_F, &tmp_DAC_data, 0);
	//printf("PCB Temperature = %.1fC ", PCB_tmp_C);
	//printf("(%.1fF) \n", PCB_tmp_F);
}

void speed_ctrl_knob_read(){
	//set up I2C address
	extern uint16_t I2C_addr;
	I2C_addr = ADS101x_I2C_addr_EXT;
	I2C_set_addr(&I2C_addr);

	extern struct ADS101x_para_obj ADS101x_para;	//define as a global variable in ADS101x.c
	extern uint16_t ADC_channel;
	ADC_channel = ADS1015_MUX_SE_IN3;	//ADC channel 3 is connected to speed knob
	ADS101x_single_volt_read(&ADS101x_para, &ADC_channel);
	//printf(" ADC data = %d \n", ADS101x_para->ADC_data);
	//printf(" ADC FSR = %.3fV \n", ADS101x_para->ADC_FSR);
	//printf("speed knob = %.3fV \n", ADS101x_para.ADC_volt_read);

	PWM_val_read = (int16_t) (PWM_abs_min + (PWM_abs_max-PWM_abs_min) * ADS101x_para.ADC_volt_read/ADS101x_max_volt);
	if(PWM_val_read > PWM_abs_max){
		PWM_val_read = PWM_abs_max;
	}else if(PWM_val_read < PWM_abs_min){
		PWM_val_read = PWM_abs_min;
	}
	//printf("PWM = %d \n", PWM_val_read);
}

void acce_ctrl_knob_read(){	//control acceleration
	//set up I2C address
	extern uint16_t I2C_addr;
	I2C_addr = ADS101x_I2C_addr_EXT;
	I2C_set_addr(&I2C_addr);

	extern struct ADS101x_para_obj ADS101x_para;	//define as a global variable in ADS101x.c
	extern uint16_t ADC_channel;
	ADC_channel = ADS1015_MUX_SE_IN2;	//ADC channel 2 is connected to acce knob
	ADS101x_single_volt_read(&ADS101x_para, &ADC_channel);
	//printf(" ADC data = %d \n", ADS101x_para->ADC_data);
	//printf(" ADC FSR = %.3fV \n", ADS101x_para->ADC_FSR);
	//printf("acceleration knob = %.3fV \n", ADS101x_para.ADC_volt_read);

	acce_val_read = PWM_ramp_up_abs_min + (PWM_ramp_up_abs_max-PWM_ramp_up_abs_min) * ADS101x_para.ADC_volt_read/ADS101x_max_volt;
	//printf("acce_val_read = %.3f \n", acce_val_read);
}

//--------------------------------------------------------------------
void HW(){
	printf("Hello World!!!\n");
}

void GPIO_test(int PIN){   
    if(!bcm2835_init()){   //initialize BCM2835 GPIO
        printf("fail to initialize BCM2835");
    }
    bcm2835_gpio_fsel(PIN, BCM2835_GPIO_FSEL_OUTP); //set PIN as output
    while(1){
        bcm2835_gpio_write(PIN, HIGH);  //pull output high
        delay(500);
        bcm2835_gpio_write(PIN, LOW);  //pull output low
        delay(500);
    }
} 

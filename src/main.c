#include <bcm2835.h>
#include <stdio.h>
#include "e_car_ctrl.h"

/* Notes:
   1. GPIO 13 cannot used as input for reading; bad pin? or bcm2835 library problem?
*/


int main(int argc, char **argv){
	e_car_init();	//set up raspberry pi pin paramemeter

	int pwm_data = 0;
	uint8_t Motor_DIR_val = 0;
	uint8_t pre_PEDAL_val = 0;
	uint8_t PEDAL_val = 0;

	while(1){
		Motor_DIR_val = bcm2835_gpio_lev(Motor_DIR_PIN_IN);	//reading switch state
		if(Motor_DIR_val == 0)
			bcm2835_gpio_write(Motor_DIR_PIN_OUT, HIGH);	//change direction
		else
			bcm2835_gpio_write(Motor_DIR_PIN_OUT, LOW);	//change direction
		printf("Motor_DIR_val = %d\n", Motor_DIR_val);
		
		PEDAL_val = bcm2835_gpio_lev(PEDAL_PIN); //reading Pedal state
		if(PEDAL_val == 1){		//Pedal press detected; speed up
			if(PEDAL_val != pre_PEDAL_val){	//start from init_speed instead of 0
				pwm_data = init_speed;	
			}
			if(pwm_data < max_speed - 1){	//check for max speed
				pwm_data += PWM_DCIR;	//increase duty cycle
			}
			bcm2835_pwm_set_data(PWM_CHANNEL, pwm_data);	//write duty cycle
		} else if(PEDAL_val == 0){	//Pedal is released; slow down
			if(pwm_data > init_speed){	//check for initial speed
				pwm_data -= PWM_DCIR*SS_ratio;	//decrease duty cycle
			} else {
				pwm_data = 0;	//stop motor completely
			}
			bcm2835_pwm_set_data(PWM_CHANNEL, pwm_data);	//write duty cycle
		} else {	//just in case something goes wrong; turn off motor
			bcm2835_pwm_set_data(PWM_CHANNEL, 0);
		}
		pre_PEDAL_val = PEDAL_val;	//record previous Pedal state
		printf("pwm_data = %d\n", pwm_data);
		printf("Pedal val = %d\n", PEDAL_val);

		delay(PWM_delay);	//delay in ms
	}

	bcm2835_close();
	return 0;
}


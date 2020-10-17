#include <stdio.h>
#include "e_car_ctrl.h"
#include <bcm2835.h>

int e_car_init(){
	printf("Initializing e_car parameters...\n");
	
	//Set GPIO pin as output
	bcm2835_gpio_fsel(DRV8343_EN, BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_fsel(PWM_INLx_EN, BCM2835_GPIO_FSEL_OUTP);
	printf("Enabling DRV8343 Gate Driver \n");	
	bcm2835_gpio_write(DRV8343_EN, HIGH);	//Enable DRV8343 Gate Driver
	//Enable PWM in 3xPMW Mode; check DS P21 for more detail 
	//If INLx is not at high, PWM signal will not reach the gate each MOSFET
	bcm2835_gpio_write(PWM_INLx_EN, HIGH);	

	//set GPIO pin as input and set up pull up or pull down
	bcm2835_gpio_fsel(Motor_DIR_PIN_IN, BCM2835_GPIO_FSEL_INPT);
	bcm2835_gpio_set_pud(Motor_DIR_PIN_IN, BCM2835_GPIO_PUD_UP);	//pull up
	bcm2835_gpio_fsel(PEDAL_PIN, BCM2835_GPIO_FSEL_INPT);
	bcm2835_gpio_set_pud(PEDAL_PIN, BCM2835_GPIO_PUD_UP);	//pull up
	bcm2835_gpio_fsel(DRV8343_FLT, BCM2835_GPIO_FSEL_INPT);
	//cannot use pull up here; input is at 1.2V when Fault is active (low)
	//there is a 10k pull-up on the board
	//it looks like DRV8343 open drain pin does not have enough strength to pull the line low when Raspberry Pi pull-up is enabled
	//The downside of this is that this programm will keep reporting DRV8343 Fault Flag when the line is not connected
	bcm2835_gpio_set_pud(DRV8343_FLT, BCM2835_GPIO_PUD_DOWN);	//pull down

	//Set the GPIO pin to the correct Alt Fun in order to use PWM channel 0 at the pin
	bcm2835_gpio_fsel(PWM_INHA, BCM2835_GPIO_FSEL_ALT0);
	bcm2835_gpio_fsel(PWM_INHB, BCM2835_GPIO_FSEL_ALT0);

	//Set clock divider to 16
	//Set range to 1024 in Markspace Mode
	//PRF=(19.2MHz/16)/1024=1171.875Hz
	printf("Setting PWM Parameter \n");
	bcm2835_pwm_set_clock(BCM2835_PWM_CLOCK_DIVIDER_16);
	bcm2835_pwm_set_mode(PWM_CHANNEL0, 1, 1);
	bcm2835_pwm_set_range(PWM_CHANNEL0, RANGE);
	bcm2835_pwm_set_mode(PWM_CHANNEL1, 1, 1);
	bcm2835_pwm_set_range(PWM_CHANNEL1, RANGE);
	printf("e_car Initialization Completed!\n");
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

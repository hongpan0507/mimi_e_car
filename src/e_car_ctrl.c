#include <stdio.h>
#include "e_car_ctrl.h"
#include <bcm2835.h>

int e_car_init(){
	printf("Initializing parameters...\n");
	
	if(!bcm2835_init()){
		printf("Failed to initialize BCM2835 library");
		return 1;
	}

	//Set GPIO pin as output
	bcm2835_gpio_fsel(Motor_DIR_PIN_OUT, BCM2835_GPIO_FSEL_OUTP);
	
	//set GPIO pin as input and set up pull up or pull down
	bcm2835_gpio_fsel(Motor_DIR_PIN_IN, BCM2835_GPIO_FSEL_INPT);
	bcm2835_gpio_set_pud(Motor_DIR_PIN_IN, BCM2835_GPIO_PUD_UP);	//pull up
	bcm2835_gpio_fsel(PEDAL_PIN, BCM2835_GPIO_FSEL_INPT);
	bcm2835_gpio_set_pud(PEDAL_PIN, BCM2835_GPIO_PUD_UP);	//pull up
	
	//Set the GPIO pin to Alt Fun 5 in order to use PWM channel 0 at the pin
	bcm2835_gpio_fsel(PWM_PIN, BCM2835_GPIO_FSEL_ALT5);

	//Set clock divider to 16
	//Set range to 1024 in Markspace Mode
	//PRF=(19.2MHz/16)/1024=1171.875Hz
	bcm2835_pwm_set_clock(BCM2835_PWM_CLOCK_DIVIDER_16);
	bcm2835_pwm_set_mode(PWM_CHANNEL, 1, 1);
	bcm2835_pwm_set_range(PWM_CHANNEL, RANGE);
	
	printf("Initialization Completed!\n");
}

void motor_DIR(){
	int Motor_DIR_val = bcm2835_gpio_lev(Motor_DIR_PIN_IN);	//reading switch state
		if(Motor_DIR_val == 0)
			bcm2835_gpio_write(Motor_DIR_PIN_OUT, HIGH);	//change direction
		else
			bcm2835_gpio_write(Motor_DIR_PIN_OUT, LOW);	//change direction
		printf("Motor_DIR_val = %d\n", Motor_DIR_val);

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

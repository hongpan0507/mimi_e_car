#include <stdio.h>
#include "e_car_ctrl.h"
#include <bcm2835.h>


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

#include <stdio.h>
#include <bcm2835.h>
#include "e_car_ctrl.h"

#define LED_PIN RPI_GPIO_P1_11	//Blinks LED using RasPi GPIO 11


int main(){
    HW();
    GPIO_test(LED_PIN);
}


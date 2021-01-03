#include <bcm2835.h>

// Funcations and variable Declarations
// DRV8343S SPI Physical Pin assignment: 
#define DRV8343_EN 		RPI_BPLUS_GPIO_J8_26 	//Use Pin26/GPIO7 as output to enable DRV8343S
#define DRV8343_FLT 	RPI_BPLUS_GPIO_J8_22 	//Use Pin22/GPIO25 as input to detect DRV8343S Fault alert

// TMP275 temperature limit alert pin
#define Power_MOSFET_over_tmp_alert RPI_BPLUS_GPIO_J8_07 

// PWM Pin assignment
// For pins to be used as PWM, it must be set to the correct "Alt Fun"
// From BCM2835 documentation; GPIO 12, 13, 18, 19 can be used as PWM; check documentation for details
// GPIO 40, 41, 45, 52, 53 have PWM function, but the physical pin cannot be located in Raspberry pi 3 
#define PWM_INHA 		RPI_BPLUS_GPIO_J8_32 	//Use Pin32/GPIO12 as PWM pin to drive DC motor
#define PWM_INHB 		RPI_BPLUS_GPIO_J8_33 	//Use pin33/GPIO13 as PWM to drive DC Motor
#define PWM_INHC 		RPI_BPLUS_GPIO_J8_12	// Use pin12/GPIO18 as PWM to control something else; need to set GPIO ALT FUN to 5 to use PWM
#define PWM_INLx_EN 	RPI_BPLUS_GPIO_J8_11 	//Use pin11/GPIO17 as output to enable PWM in 3xPWM mode; check DS P21 for more detail 

#define Motor_DIR_PIN_IN 	RPI_BPLUS_GPIO_J8_16	//Use Pin16/GPIO23  as input to read a switch input to change motor direction 
#define PEDAL_PIN 			RPI_BPLUS_GPIO_J8_13	//Use Pin13/GPIO27 on J8 header as input to start or stop motor 

#define PWM_CHANNEL0 	0
#define PWM_CHANNEL1 	1
#define PWM_RANGE 		1024				//full duty cycle = 1024
#define PWM_delay 		10					//duty cycle increase delay in ms
#define PWM_DCIR 		1					//PWM Duty Cycle Increase Rate
#define init_speed 		0.1*PWM_RANGE		//initial speed
#define max_speed 		1.0*PWM_RANGE		//Top speed
#define SS_ratio 		3					//start/stop duty cycle change ratio; need to stop faster than start

#define PWM_init		0.1*PWM_RANGE		// initial_PWM = initial speed
#define PWM_max			0.6*PWM_RANGE		// max_PWM = top speed	
#define PWM_ramp_rate   1					// 
#define PWM_time_unit	3000				// control how fast PWM value is updated; found by trial and error; highly dependant on loop execution time
#define coast_ON		1
#define coast_OFF		0
#define fan_ON	1
#define fan_OFF	0


void HW();  //For testing only
void GPIO_test(int PIN);    //For hardware GPIO pin function testing
int e_car_init();	//pin setup
void motor_DIR();	//read and change motor direction
//void motor_speed_ctrl_linear(int *PWM_val, uint16_t *ramp_rate, uint16_t *init_PWM_val, uint16_t *time_count);
void motor_speed_ctrl_linear(uint16_t *PWM_val, float *ramp_rate, uint16_t *init_PWM_val,  uint32_t *time_count);
void motor_move(uint16_t *PWM_val, uint8_t *motor_DIR_val);
void motor_brake();	//hard stop when E-stop button is pushed
void motor_coast(uint8_t CTRL);
void power_MOSFET_cooling_fan_CTRL(uint8_t CTRL);

// Funcations and variable Declarations
// DRV8343S SPI Physical Pin assignment: 
#define DRV8343_EN RPI_GPIO_P1_26		//Use Pin26/GPIO7 as output to enable DRV8343S
#define DRV8343_FLT RPI_GPIO_P1_22		//Use Pin22/GPIO25 as input to detect DRV8343S Fault alter

// PWM Pin assignment
// For pins  be used as PWM, it must be set to the correct "Alt Fun"
// From BCM2835 documentation; GPIO 12, 13, 18, 19 can be used as PWM; check documentation for details
// GPIO 40, 41, 45, 52, 53 have PWM function, but the physical pin cannot be located in Raspberry pi 3 
#define PWM_INHA RPI_V2_GPIO_P1_32		//Use Pin32/GPIO12 as PWM pin to drive DC motor
#define PWM_INHB RPI_V2_GPIO_P1_33		//Use pin33/GPIO13 as PWM to drive DC Motor
#define PWM_INHC RPI_V2_GPIO_P1_35		// Use pin35/GPIO19 as PWM to control something else; need to set GPIO ALT FUN to 5 to use PWM
#define PWM_INLx_EN RPI_GPIO_P1_11		//Use pin11/GPIO17 as output to enable PWM in 3xPWM mode; check DS P21 for more detail 

#define Motor_DIR_PIN_IN RPI_GPIO_P1_16	//Use Pin16/GPIO23  as input to read a switch input to change motor direction 
//it was pin 22 was used as PEDAL detection; need to change wiring
#define PEDAL_PIN RPI_GPIO_P1_18	//Use GPIO 18 as input to start or stop motor 

#define PWM_CHANNEL0 0
#define PWM_CHANNEL1 1
#define RANGE 1024		//full duty cycle = 1024
#define PWM_delay 10		//duty cycle increase delay in ms
#define PWM_DCIR 1	//PWM Duty Cycle Increase Rate
#define init_speed 0.1*RANGE	//initial speed
#define max_speed 0.6*RANGE		//Top speed
#define SS_ratio 3	//start/stop duty cycle change ratio; need to stop faster than start

void HW();  //For testing only
void GPIO_test(int PIN);    //For hardware GPIO pin function testing
int e_car_init();	//pin setup
void motor_DIR();	//read and change motor direction
void speed_up();	//speed up when pedal is pressed
void slow_down();	//slow down when the pedal is  not pressed
void hard_stop();	//hard stop when E-stop button is pushed

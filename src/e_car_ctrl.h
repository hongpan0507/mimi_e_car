// Funcations and variable Declarations
#define Motor_DIR_PIN_OUT RPI_GPIO_P1_11	//Use GPIO 11 as output to change  Motor direction 
#define Motor_DIR_PIN_IN RPI_GPIO_P1_16	//Use GPIO 16 as input to read a switch input to change motor direction 
#define PEDAL_PIN RPI_GPIO_P1_22	//Use GPIO 22 as input to start or stop motor 
#define PWM_PIN RPI_GPIO_P1_12	//Use GPIO 12 as PWM pin to drive DC motor
#define PWM_CHANNEL 0
#define RANGE 1024		//full duty cycle = 1024
#define PWM_delay 10		//duty cycle increase delay in ms
#define PWM_DCIR 1	//PWM Duty Cycle Increase Rate
#define init_speed 0.1*RANGE	//initial speed
#define max_speed 0.6*RANGE		//Top speed
#define SS_ratio 2	//start/stop duty cycle change ratio; need to stop faster than start

void HW();  //For testing only
void GPIO_test(int PIN);    //For hardware GPIO pin function testing
int e_car_init();	//pin setup
void motor_DIR();	//read and change motor direction
void speed_up();	//speed up when pedal is pressed
void slow_down();	//slow down when the pedal is  not pressed
void hard_stop();	//hard stop when E-stop button is pushed

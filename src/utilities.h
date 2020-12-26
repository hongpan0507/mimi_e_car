/*
	A collection of useful C language functions
	Created on 11/29/2020
	Updated on 11/29/2020
   		1. Added signed_12bit_to_signed_16bit(short *num_16bit);
			* 12 bit signed number to 16 bit signed number conversion
		2. Added temperature farenheit to celsus conversion and vice versa
	Updated on 12/25/2020
		1. Added print_char_array();
			* print char array element one by one on the same line
		2. Added print_as_bin();
			*print data as binary
*/

//mask for detecting 12-bit signed number stored in more than 12-bit data type
#define bit12_mask				0x0800	
#define num_12bit_erase_mask	0x0FFF

void print_char_array(char* txt, int txt_len);
void print_as_bin(short *data);
int signed_12bit_to_signed_16bit(short *num_16bit);
float *C_to_F(float *temp_C);
float *F_to_C(float *temp_F);

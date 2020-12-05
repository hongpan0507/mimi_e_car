/*
   A collection of useful C language functions
   Created on 11/29/2020
   Updated on 11/29/2020
   		1. Added signed_12bit_to_signed_16bit(short *num_16bit);
			* 12 bit signed number to 16 bit signed number conversion
*/

//mask for detecting 12-bit signed number stored in more than 12-bit data type
#define bit12_mask				0x0800	
#define num_12bit_erase_mask	0x0FFF

int signed_12bit_to_signed_16bit(short *num_16bit);

#include "utilities.h"
#include <stdio.h>

// convert signed 12 bit number stored in 16 bit long data to a negative number
int signed_12bit_to_signed_16bit(short *num_16bit){
	if (sizeof(*num_16bit)==sizeof(short)){
		// detect if bit 12 is 1, if so, it is a negative number
		if ((*num_16bit & bit12_mask) == bit12_mask ){	
			// "short" data type occupies 2 bytes of RAM; 2 byte = 16 bit
			// 2s complement convert to positive number
			// flip all the bits and then add 1 = positive representation of the negative number
			// times by -1 to get the negative number
			*num_16bit = ~(*num_16bit);	// bitwise flip all the bits
			*num_16bit = *num_16bit+1;	 
			*num_16bit = *num_16bit & num_12bit_erase_mask; // erase all the bits pass bit 12
			*num_16bit = -1 * (*num_16bit);
		}
	} else {
		fprintf(stderr, "***error occurred for \'signed_12bit_to_signed_16bit\'*** \n");
		fprintf(stderr, "***error message start*** \n");
		fprintf(stderr, "No conversion was done \n");
		fprintf(stderr, "incorrect data bit size \n");
		fprintf(stderr, "bit size must be 16 bit \n");
		fprintf(stderr, "***error message end*** \n");
	}
	return 0;
}


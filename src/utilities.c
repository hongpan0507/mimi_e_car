#include "utilities.h"
#include <stdio.h>

void print_char_array(char* txt, int txt_len){
//	printf("size of array: %d \n", sizeof(txt));
//	printf("size of array element: %d \n", sizeof(txt[0]));
	for(int i=0; i<txt_len; i++){
		//printf("%c", txt[i]);	// print as char
		printf("%d", (short)txt[i]);	// print as integer
	}
	printf("\n");
}

// take in short (2 byte long) for now
// for other data type, change *data from short to desired data type
void print_as_bin(short *data){
	short mask = 1;	// use for erasing all bits except LSB
	short bin = 0;
	//printf("bit size: %d \n", sizeof(*data)*8);
	for(int i=0; i<sizeof(*data)*8; i++){ // 1 byte = 8 bits
		//printf("value of i: %d \n", i);
		//print MSB first
		//convert from byte size to bit size
		//shift bits from left to the LSB and print
		bin = (*data >> (sizeof(*data)*8 - 1 - i)) & mask;
		//printf("%d\n", bin);
		printf("%d", bin);
		if((i+1)%4 == 0){
			printf(" ");	// print empty space for every 4 bits
		}
	}
	printf("\n");
}

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

void *C_to_F(float *temp_C, float *temp_F){
	*temp_F = *temp_C * 9/5 + 32;
}

void *F_to_C(float *temp_C, float *temp_F){
	*temp_C = (*temp_F - 32) * 5/9;
}

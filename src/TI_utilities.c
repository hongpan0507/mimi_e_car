// library for collection of useful function for TI devices
// Created on 1/22/2021 by hpan

// good for 12-bit data stored in 16-bit register with LS 4 bits not used
// convert char buff[2] received by I2C to short
// each char is 8 bits, each short is 16 bits, char array cannot be greater than 2
void _2char_to_12bit(char *data_char, short *data_num){
	*data_num = 0;	// make sure the content is zero before using it because bitwise OR is senitive to it
	*data_num = *data_num | data_char[0];	//copy the most significant byte
	*data_num = *data_num << 8; //left shift the most signficiant byte by 8 bit
	*data_num = *data_num | data_char[1];	//copy the least significant byte
	// For TI chips, the LS 4 bit is usually not used
	// need to right shift by 4 bit to get the data_num to the correct bit field
	*data_num = *data_num >> 4;
	//*data_num = *data_num & 0x0FFF;		//erase MS 4 bits in case when they're not zero
}

// good for 16-bit data stored in 16-bit register 
// convert data_char[2] received by I2C to short
// each char is 8 bits, each short is 16 bits, char array cannot be greater than 2
void _2char_to_16bit(char *data_char, short *data_num){
	*data_num = 0;	// make sure the content is zero before using it because bitwise OR is senitive to it
	*data_num = *data_num | data_char[0];	//copy the most significant byte
	*data_num = *data_num << 8; //left shift the most signficiant byte by 8 bit
	*data_num = *data_num | data_char[1];	//copy the least significant byte
}

// convert short (16-bit) to char data[2] for writing temperature limit to the register
// each char is 8 bits, each short is 16 bits, char array cannot be greater than 2
// no need to do anything special to a negative number 
// data_char[0] = MSB; data_char[1] = LSB
void _16bit_to_2char(char *data_char, short *data_num){
	data_char[0] = (char) (*data_num >> 8); //right shift by 8 bit to get most significant 8 bit into char [0]
	data_char[1] = (char) *data_num; //only keep the LS 8 bits
}

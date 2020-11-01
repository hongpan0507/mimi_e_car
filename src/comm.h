#include <stdbool.h>

// initialize communication protocal
// provides communication functions
// SPI, I2C

// SPI Physical Pin wiring:
// Pin19=MOSI; Pin21=MISO; Pin23=SCLK; Pin24=ChipSelect

#define DRV8343_SPI_read_addr_mask 		(0b10000000)	
#define DRV8343_SPI_write_addr_mask 	(0b01111111)
#define DRV8343_SPI_read_data_mask		(0x00FF)
#define DRV8343_SPI_data_byte_len		2

int DRV8343_SPI_init();
int DRV8343_SPI_read(uint16_t *addr, uint16_t *read_data, bool verbose);
int DRV8343_SPI_write(uint16_t *addr, uint16_t *write_data, uint16_t *read_data, bool verbose);


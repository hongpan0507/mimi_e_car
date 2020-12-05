#include <stdbool.h>

// initialize communication protocal
// provides communication functions
// SPI, I2C

// SPI Physical Pin wiring:
// Pin19=MOSI; Pin21=MISO; Pin23=SCLK; Pin24=ChipSelect

// I2C 1 Phsycial Pin wiring:
// Pin27/GPIO0=SDA; Pin28/GPIO1=SCL; Pin29/GPIO5=Alt1

#define DRV8343_SPI_read_addr_mask 		(0b10000000)	
#define DRV8343_SPI_write_addr_mask 	(0b01111111)
#define DRV8343_SPI_read_data_mask		(0x00FF)
#define DRV8343_SPI_data_byte_len		2
#define I2C2_ADS1015_slave_addr 		(0b1001001)		// I2C 2; on-board voltage monitoring
#define I2C1_ADS1015_slave_addr			(0b1001001)		// I2C 1; external sensor monitoring

int DRV8343_SPI_init();
int DRV8343_SPI_read(uint16_t *addr, uint16_t *read_data, bool verbose);
int DRV8343_SPI_write(uint16_t *addr, uint16_t *write_data, uint16_t *read_data, bool verbose);

int I2C_init();

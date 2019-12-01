#ifndef __COMMON_H__
#define __COMMON_H__

typedef struct pin_s
{
	char[5] 	port;
	char[10]	description;
} pin_t;

typedef struct uart_pin_s
{
	int id;		
	char[5] tx;
	char[5]	rx;
} uart_pin_t;

typedef struct iic_pin_s
{
	int id;		
	char[5] scl;
	char[5]	sda;
} iic_pin_t;

typedef struct spi_pin_s
{
	int id;		
	char[5] mosi;
	char[5]	miso;
	char[5]	sck;
	char[5] cs;
} spi_pin_t;



typedef struct mcu_source_management_s
{
	pin_t * pinlist;
	uart_pin_t * uartlist;
	iic_pin_t * iiclist;
	spi_pin_t * spilist;
} mcu_source_management_t;

#endif
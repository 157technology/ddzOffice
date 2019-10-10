#ifndef __SERIAL_H__
#define __SERIAL_H__ 

#include "main.h"
#include "usart.h"
#include <stdio.h>
#include <stdarg.h>

void em_printf(const char * format, ...);

typedef struct 
{
	UART_HandleTypeDef * puart;

	char * revData;
	uint16_t size;

	int (* Initialize) (UART_HandleTypeDef * huart, uint16_t size);
} SerialDev;

//extern SerialDev serialDev;
SerialDev * registerSerial(UART_HandleTypeDef * huart, uint16_t size);

#endif
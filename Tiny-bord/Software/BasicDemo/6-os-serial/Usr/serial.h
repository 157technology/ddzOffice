#ifndef __SERIAL_H__
#define __SERIAL_H__ 

#include "main.h"
#include "usart.h"
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

#include "cmsis_os2.h"

void em_printf(const char * format, ...);

typedef struct em_serial_dev
{
	UART_HandleTypeDef * puart;

	char * rbuf;
	uint16_t rbufSize;
	uint16_t rCnt;
	

	char * tbuf;
	uint16_t tbufSize;


	osSemaphoreId_t * pSemSerial;

	void (* RestartDma) (struct em_serial_dev *serial);
	int (* Initialize) (struct em_serial_dev *serial);
} SerialDev;

//extern SerialDev serialDev;
SerialDev * registerSerial(UART_HandleTypeDef * huart, uint16_t rbufsize, uint16_t tbufsize, osSemaphoreId_t * pSemSerial);


int initSerial(SerialDev * serial);

#endif
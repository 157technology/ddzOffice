#ifndef __SERIAL_H__
#define __SERIAL_H__ 

#include "serial_config.h"
#include "shell_config.h"
#include "memory.h"

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

#ifdef USING_OS
#include "cmsis_os2.h"
#endif

#ifdef USING_SHELL
#include "shell.h"
#endif



/**/
void em_printf(const char * format, ...);

typedef struct em_serial_dev
{
	UART_HandleTypeDef * puart;

	char * rbuf;
	uint16_t rbufSize;
	uint16_t rCnt;
	

	char * tbuf;
	uint16_t tbufSize;

	#ifdef USING_OS
	osSemaphoreId_t * pSemSerial;
	#endif

	void (* RestartDma) (struct em_serial_dev *serial);
	void (* Initialize) (struct em_serial_dev *serial);
	void (* print)		(const char * format, ...);
} SerialDev_t;

//extern SerialDev serialDev;
SerialDev_t * registerSerial(UART_HandleTypeDef * huart, uint16_t rbufsize, uint16_t tbufsize, osSemaphoreId_t * pSemSerial);




#endif

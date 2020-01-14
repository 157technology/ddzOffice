/*
 * @Author: your name
 * @Date: 2020-01-14 10:42:46
 * @LastEditTime : 2020-01-14 17:15:17
 * @LastEditors  : Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \demo\EM_Device\serial.h
 */
#ifndef __SERIAL_H__
#define __SERIAL_H__

#include "usart.h"
#include "../Core/Inc/main.h"
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include "signal_slot.h"
#include "memory.h"
#define CONSOLE_UART huart1
#define CONSOLE_ISR USART1_IRQHandler

#define WIFI_UART huart6
#define WIFI_ISR USART6_IRQHandler





typedef struct __serial
{
	UART_HandleTypeDef *puart;
	char *rbuf;
	int rbufSize;
	int rCnt;

	char *tbuf;
	int tbufSize;

	pfun signal;
	/**/
	
} Serial;

extern Serial *console;

void em_printf(const char *format, ...);

Serial *Serial_Regester(UART_HandleTypeDef *puart, int rbufSize, int tbufSize);
void Serial_Init(Serial *ps);
void Serial_Print(Serial *ps, const char *format, ...);




void emHAL_UART_Transmit_DMA(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size);

#endif

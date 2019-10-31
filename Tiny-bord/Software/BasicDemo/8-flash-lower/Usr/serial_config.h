#ifndef __SERIAL_CONFIG_H__
#define __SERIAL_CONFIG_H__ 

#include "main.h"
#include "usart.h"

#define USING_OS	1

#define SHELL_UART		huart1
#define SHELL_ISR		USART1_IRQHandler

#endif
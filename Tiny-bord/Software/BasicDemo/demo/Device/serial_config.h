#ifndef __SERIAL_CONFIG_H__
#define __SERIAL_CONFIG_H__

#include "main.h"
#include "usart.h"

#define USING_OS	1

#ifdef __cplusplus
extern "C" {
#endif

/* 定义UART设备 */


#define CONSOLE_UART	huart1
#define CONSOLE_ISR		USART1_IRQHandler

// #define SHELL_UART		huart6
// #define SHELL_ISR		USART6_IRQHandler

#ifdef __cplusplus
}
#endif

#endif
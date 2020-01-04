#ifndef __SERIAL_H__
#define __SERIAL_H__







#ifdef __cplusplus


/* 定义UART设备 */
#define USING_OS	1

#define CONSOLE_UART	huart1
#define CONSOLE_ISR		USART1_IRQHandler

#define WIFI_UART		huart6
#define WIFI_ISR		USART6_IRQHandler

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include "memory.h"
#include "signalslot.h"
#include <map>
#include <string>
using namespace std;
typedef map<string, UART_HandleTypeDef*> 	uart_map_t;

class Serial: public Object
{
public:
	Serial(string com);
	~Serial();

private:
	string m_com;	//UART模块

	char * rbuf;	//接收缓存地址
	int	rbufSize;	//接收缓存区大小
	int rCnt;		//接收数据个数

	char * tbuf;	//发送缓存地址
	int tbufSize;	//发送缓存区大小

	/* private function */

public:
	bool init();
	bool open();

	void print(const char *format, ...);

	/* signal--emit */
	void readyRead(char * data);
	void emit_readyRead(char * data);


	/* 仅用于底层ISR */
	void isr_handle(int ndtr);
};

void registerUartDev();


#endif

#endif

#ifndef __SERIAL_H__
#define __SERIAL_H__

#include "serial_config.h"
#include "memory.h"
//#include "shell_config.h"

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>



#ifdef USING_OS
#include "cmsis_os2.h"
#endif

#ifdef USING_SHELL
#include "shell.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

/**/
void em_printf(const char * format, ...);


typedef struct __uart_dev
{
	UART_HandleTypeDef * puart;
	char com[10];
} uart_dev_t;

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


	void (* RestartDma) 	(struct em_serial_dev *serial);
	void (* Initialize) 	(struct em_serial_dev *serial);
	void (* Config)			(uint32_t baud, int databits, int stopbits, int parity);
	void (* ConfigBaudRate)	(uint32_t baud);
	void (* print)			(const char * format, ...);
} SerialDev_t;

//extern SerialDev serialDev;
SerialDev_t * registerSerial(UART_HandleTypeDef * huart, uint16_t rbufsize, uint16_t tbufsize, osSemaphoreId_t * pSemSerial);

void hasData(void * data);

#ifdef __cplusplus
}
#endif


#ifdef __cplusplus
#include "signalslot.hpp"
#include <map>
#include <string>
using namespace std;
typedef map<string, UART_HandleTypeDef*> 	uart_map_t;

class Serial: public Object
{
public:
	Serial();
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

class beep: public Object
{
public:
	beep() {};

	void dealPress(string str)
	{
		printf("Get Key:: %s press\n", str.c_str());
	}
	void dealSerial(char * str)
	{
		printf("Get Serial:: %s\n", str);
	}	
};
#endif

#endif

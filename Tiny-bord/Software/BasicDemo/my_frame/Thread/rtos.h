#ifndef __RTOS_H__
#define __RTOS_H__ 

#include "RTE_Components.h"
#include  CMSIS_device_header
#include "cmsis_os2.h"

#include "serial.h"

void rtosInit(void);
void Init_GUIThread(void);



/**/
int Init_Temper_Thread(void);
extern osThreadId_t tid_Temper_Thread; 


int Init_Console_Thread(void);
extern osThreadId_t console_Thread;
extern osSemaphoreId_t semConsole;
extern SerialDev_t * consoleDev;

int Init_Wifi_Thread(void);
extern osThreadId_t tid_Wifi_Thread; 
extern osSemaphoreId_t semWifi;
extern SerialDev_t * wifiDev; 
 
#endif
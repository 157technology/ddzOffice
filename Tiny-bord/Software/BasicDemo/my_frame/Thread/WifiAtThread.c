#include "cmsis_os2.h"                          // CMSIS RTOS header file


#include "usart.h"
#include "serial.h"
#include "ui_print.h"

osSemaphoreId_t semWifi;
SerialDev_t * wifiDev;

osThreadId_t tid_Wifi_Thread;                        // thread id

static void Thread (void *argument);                   // thread function

int Init_Wifi_Thread(void)
{
	tid_Wifi_Thread = osThreadNew(Thread, NULL, NULL);
	if (tid_Wifi_Thread == NULL)
	{
		return (-1);
	}

	return (0);
}

static void Thread (void *argument)
{
	semWifi = osSemaphoreNew(1U, 0U, NULL);
	wifiDev = registerSerial(&huart1, 1024, 512, &semWifi);
	wifiDev->Initialize(wifiDev);
	while (1)
	{
		osSemaphoreAcquire(semWifi, osWaitForever);
		em_printf("Wifi>>> %s\r\n", wifiDev->rbuf);
	}
}

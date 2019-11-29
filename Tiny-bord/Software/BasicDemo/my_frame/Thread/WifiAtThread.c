#include "cmsis_os2.h"                          // CMSIS RTOS header file


#include "usart.h"
#include "serial.h"
#include "ui_print.h"
#include "wifi.h"

osSemaphoreId_t semWifi;
SerialDev_t * wifiDev;

osThreadId_t tid_Wifi_Serial_Thread;	// thread id

osThreadId_t tid_Wifi_Thread;			// thread id

static void wifi_serial(void *argument);	// thread function
static void wifi_fsm(void *argument);	// thread function

int Init_Wifi_Thread(void)
{
	tid_Wifi_Serial_Thread = osThreadNew(wifi_serial, NULL, NULL);
	tid_Wifi_Thread	= osThreadNew(wifi_fsm, NULL, NULL);
	if (tid_Wifi_Serial_Thread == NULL || tid_Wifi_Thread == NULL )
	{
		return (-1);
	}

	return (0);
}

static void wifi_serial(void *argument)
{
	semWifi = osSemaphoreNew(1U, 0U, NULL);
	wifiDev = registerSerial(&huart1, 1024, 1024, &semWifi);
	wifiDev->Initialize(wifiDev);
	osDelay(2000);
	wifi.init();

	//wifi.setMode(Wifi_STA);

	//wifiTest();

	while (1)
	{
		osSemaphoreAcquire(semWifi, osWaitForever);
		//em_printf("Wifi>>> %s\r\n", wifiDev->rbuf);
		wifi.recieve(wifiDev->rbuf);
	}
}

static void wifi_fsm(void *argument)
{
	while ( 1 )
	{
		osDelay(5000);
		wifi.fsm();

		//showStatus();
	}
}

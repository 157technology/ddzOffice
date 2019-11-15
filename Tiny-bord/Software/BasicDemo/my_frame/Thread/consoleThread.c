#include "cmsis_os2.h"                          // CMSIS RTOS header file

#include "usart.h"
#include "serial.h"
#include "ui_print.h"

/*
shell test
*/
#include "shell.h"
/*----------------------------------------------------------------------------
 *      Thread 1 'Thread_Name': Sample thread
 *---------------------------------------------------------------------------*/
osSemaphoreId_t semConsole;
SerialDev_t * consoleDev;

osThreadId_t console_Thread;                        // thread id

static void Thread (void *argument);                   // thread function

int Init_Console_Thread(void)
{
	console_Thread = osThreadNew(Thread, NULL, NULL);
	if (console_Thread == NULL)
	{
		return (-1);
	}
	return (0);
}

static void Thread (void *argument)
{
	semConsole = osSemaphoreNew(1U, 0U, NULL);
	consoleDev = registerSerial(&huart2, 1024, 512, &semConsole);
	consoleDev->Initialize(consoleDev);
	em_printf("Init success.\n");

	shell_init();
	//shell_history_init();
	while (1)
	{
		osSemaphoreAcquire(semConsole, osWaitForever);
		//ui_printf("> %s", consoleDev->rbuf);
		//em_printf("> %s", consoleDev->rbuf);
		
		shell_handle(consoleDev->rbuf);
		
		
//		for ( int i = 0; i < consoleDev->rCnt; i ++ )
//		{
//			em_printf("%d --> 0x%X.\n", i, consoleDev->rbuf[i]);
//		}
		
		//wf_printf("AT\r\n");
		/**/
	}
}

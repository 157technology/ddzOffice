#include "cmsis_os2.h"                          // CMSIS RTOS header file

#include <stdio.h>
#include "GUI.h"
#include "ds18b20.h"
#include "ui_print.h"

osThreadId_t tid_Temper_Thread;                        // thread id

static void Thread (void *argument);                   // thread function

int Init_Temper_Thread(void)
{
	tid_Temper_Thread = osThreadNew(Thread, NULL, NULL);
	if (tid_Temper_Thread == NULL)
	{
		return (-1);
	}

	return (0);
}

static void Thread (void *argument)
{
	char str[20];
	while (1)
	{
		sprintf(str, "T :: %.2lf", Temper18b20.temper());
		GUI_DispStringAtCEOL(str, 0, 0);
		osDelay(500);                           // suspend thread
	}
}

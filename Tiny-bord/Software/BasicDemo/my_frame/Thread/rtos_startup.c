#include "rtos.h"

#include "led.h"

#include "GUI.h"
#include "ui_print.h"

#include "oled.h"

void app_main (void *argument)
{
    //Init_GUIThread();
//	GUI_SetBkColor(GUI_WHITE);
//	GUI_SetColor(GUI_BLACK);
	Init_Console_Thread();
    Init_Temper_Thread();
	Init_Wifi_Thread();
    for (;;) 
	{
        //ledDrv.leftstream(&ledDrv, 0);
		OLED_Replot();
		osDelay(100);
        //ui_printf("> main running.\n");
	}
}

void rtosInit(void)
{
    // System Initialization
    SystemCoreClockUpdate();
    // ...
    osKernelInitialize();                 // Initialize CMSIS-RTOS
    osThreadNew(app_main, NULL, NULL);    // Create application main thread
    osKernelStart();                      // Start thread execution
    for (;;) {}
}

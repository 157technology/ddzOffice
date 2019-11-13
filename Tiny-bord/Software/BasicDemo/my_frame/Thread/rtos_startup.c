#include "rtos.h"

#include "led.h"

void app_main (void *argument)
{
    //Init_GUIThread();
    Init_Temper_Thread();
    for (;;) 
	{
        //ledDrv.leftstream(&ledDrv, 0);
		osDelay(1000);
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

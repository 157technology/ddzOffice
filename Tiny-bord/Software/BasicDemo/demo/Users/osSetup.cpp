#include "common.hpp"

#include "EventRecorder.h"              // Keil.ARM Compiler::Compiler:Event Recorder

#include "MyEventComponent.h"
#include "key.hpp"


extern Key btn;
extern int Init_SignalSlot_Thread (void);
extern void signalDis(void * m);
char *str = "Hello";
void app_main (void *argument)
{
	Init_SignalSlot_Thread();
	//EvrRtxMemoryInit(NULL, 0, 0);
    for (;;) 
	{
		EventRecord2 (1+EventLevelAPI, 1, 0);
		EventStartAv(10, NULL, 0);
		HAL_GPIO_TogglePin(LED3_GPIO_Port, LED3_Pin);	
		osDelay(500);
		EventRecord2 (1+EventLevelAPI, 0, 0);
		EventStopA(10);
		osDelay(500);
		//emit(signalDis, str);
	}
}

void Led_thread (void *argument)
{
    for (;;) 
	{
		HAL_GPIO_TogglePin(LED4_GPIO_Port, LED4_Pin);
		//btn.isr_handle();		
		osDelay(500);
	}
}

void osSetup(void)
{
    // System Initialization
    SystemCoreClockUpdate();
    // ...
    osKernelInitialize();                 // Initialize CMSIS-RTOS
    osThreadNew(app_main, NULL, NULL);    // Create application main thread
	osThreadNew(Led_thread, NULL, NULL);
    osKernelStart();                      // Start thread execution
    for (;;) {}
}
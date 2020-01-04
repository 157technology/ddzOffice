#include "common.hpp"

#include "EventRecorder.h"              // Keil.ARM Compiler::Compiler:Event Recorder

#include "MyEventComponent.h"
#include "key.hpp"
#include "SHT21.hpp"

#include "GUI.h"
#include "DIALOG.h"
extern Key btn;
extern int Init_SignalSlot_Thread (void);
extern void signalDis(void * m);
extern WM_HWIN myWin;
SHT21 sht21;
char *str = "Hello";
void app_main (void *argument)
{
	
	//Init_SignalSlot_Thread();
	//EvrRtxMemoryInit(NULL, 0, 0);
	int x[] = {0, 10, 20};
	int y[] = {0, 0, 10};
	int cnt = 0;
	osDelay(1000);
    for (;;) 
	{
		EventRecord2 (1+EventLevelAPI, 1, 0);
		EventStartAv(10, NULL, 0);
		HAL_GPIO_TogglePin(LED3_GPIO_Port, LED3_Pin);	
		osDelay(500);
		EventRecord2 (1+EventLevelAPI, 0, 0);
		EventStopA(10);
		osDelay(500);
		//WM_MoveTo(myWin, x[cnt], y[cnt]);
		//if ( cnt ++ > 2 )	cnt = 0;
		GUI_DispStringAt("Temper  : ", 0, 0);
		GUI_DispFloat(sht21.temper(),6);
		GUI_DispStringAt("Humidity: ", 0, 10);
		GUI_DispFloat(sht21.humidity(),6);
		
		//printf("temper:: %.2f\n", sht21.temper());
		
		//printf("humidity:: %.2f\n", sht21.humidity());
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
	Init_GUIThread();
    osThreadNew(app_main, NULL, NULL);    // Create application main thread
	osThreadNew(Led_thread, NULL, NULL);
    osKernelStart();                      // Start thread execution
    for (;;) {}
}
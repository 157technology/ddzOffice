/*----------------------------------------------------------------------------
 * CMSIS-RTOS 'main' function template
 *---------------------------------------------------------------------------*/
 
 #include "osMain.h"
 
/*----------------------------------------------------------------------------
 * Application main thread
 *---------------------------------------------------------------------------*/
 
 
 
 
 osSemaphoreId_t semSerial;
 SerialDev * emSerial;
 
 
void app_main (void *argument) {
 
  // ...
  for (;;) 
  {
  		//em_printf("This is app_main thread.\n");
	  osSemaphoreAcquire(semSerial, osWaitForever);
		em_printf("Thread>>> %s\n", emSerial->rbuf);
  }
}

int osMain (void) {
 
  // System Initialization
  SystemCoreClockUpdate();
  // ...
 
	semSerial = osSemaphoreNew(1U, 0U, NULL);
	emSerial = registerSerial(&huart1, 512, 512, &semSerial);
	emSerial->Initialize(emSerial);
	
  osKernelInitialize();                 // Initialize CMSIS-RTOS
  osThreadNew(app_main, NULL, NULL);    // Create application main thread
  osKernelStart();                      // Start thread execution
  for (;;) {}

  	return 0;
}

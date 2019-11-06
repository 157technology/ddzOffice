#include "rtx_main.h"



SerialDev_t * consoleSerial;
osSemaphoreId_t semConsole;

mem_head_t * SRAMUsed;

/**/
void app_main(void* argument)
{
	uint32_t * addr1;
	uint32_t * addr2;
	uint32_t * addr3;
	while ( 1 )
	{
		HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
		osDelay(1000);
	}
}

uint8_t buf[] = "123456789\n";

void rtx_main()
{
		// System Initialization
	MallocInit();
	HAL_UART_Transmit(&huart2, buf, 10, 999);
	//HAL_UART_Transmit_DMA(&huart2, buf, 10);
//	consoleSerial = registerSerial(&huart2, 1024, 1024, NULL);//&semConsole
//	consoleSerial->Initialize(consoleSerial);

//	consoleSerial->print("Serial Init Success.\n");
    SystemCoreClockUpdate();
    // ...
    osKernelInitialize(); // Initialize CMSIS-RTOS
    osThreadNew(app_main, NULL, NULL); // Create application main thread
    osKernelStart(); // Start thread execution
    while ( 1 );
}
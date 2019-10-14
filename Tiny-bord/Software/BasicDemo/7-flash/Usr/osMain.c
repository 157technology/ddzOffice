/*----------------------------------------------------------------------------
 * CMSIS-RTOS 'main' function template
 *---------------------------------------------------------------------------*/

#include "osMain.h"
#include "EventRecorder.h"  
/*----------------------------------------------------------------------------
 * Application main thread
 *---------------------------------------------------------------------------*/

osSemaphoreId_t semSerial;
SerialDev* emSerial;

uint16_t light = 0;
void app_main(void* argument)
{
	char * buf = emSerial->rbuf;
    for (;;) {
        osSemaphoreAcquire(semSerial, osWaitForever);
        em_printf("Thread>>> %s\n", buf);
		
        sscanf(buf, "%d", &light);
		
		em_printf("Thread>>> Light: %d.\n", light);

    }
}

void app_pwm(void* argument)
{
	uint16_t cnt = 100;
	uint16_t c = 30000;
	uint16_t cc;
    for (;;) {
//		sprintf(ss, "Flash Test: count %d", cnt ++);
//		Driver_FLASH.EraseSector(0x00);
//		Driver_FLASH.ProgramData(0x00, ss, strlen(ss));
//		Driver_FLASH.ReadData(0x00, bb, strlen(ss));
//		em_printf("flash>>> \"%s\".\n", bb);
		HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
		HAL_GPIO_TogglePin(LED2_GPIO_Port, LED2_Pin);
		HAL_GPIO_TogglePin(LED3_GPIO_Port, LED3_Pin);
		HAL_GPIO_TogglePin(LED4_GPIO_Port, LED4_Pin);
		for ( int i = 0; i < 20000-light; i ++ )	{__NOP();};
		HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
		HAL_GPIO_TogglePin(LED2_GPIO_Port, LED2_Pin);
		HAL_GPIO_TogglePin(LED3_GPIO_Port, LED3_Pin);
		HAL_GPIO_TogglePin(LED4_GPIO_Port, LED4_Pin);
		for ( int i = 0; i < light; i ++ )	{__NOP();};
    }
}


char str[] = "This is First Data in Flash";
char buf[28];

int osMain(void)
{
	
	EventRecorderInitialize (EventRecordAll, 1); 
	
    semSerial = osSemaphoreNew(1U, 0U, NULL);
    emSerial = registerSerial(&huart1, 512, 512, &semSerial);
    emSerial->Initialize(emSerial);
	
	
    em_printf("root>>> Init flash.\n");
//    ARM_Flash_Initialize(NULL);
//    ARM_Flash_EraseSector(0x00);

//    ARM_Flash_ProgramData(0x00, str, strlen(str));
//    HAL_Delay(500);
//    em_printf("root>>>wait...\n");
//    ARM_Flash_ReadData(0x00, buf, 27);
//    em_printf("root>>> Flash AT 0x00:: \"%s\".\n", buf);


//    Driver_FLASH.Initialize(NULL);
//	Driver_FLASH.EraseSector(0x00);
//	Driver_FLASH.ProgramData(0x00, str, strlen(str));
//	Driver_FLASH.ReadData(0x00, buf, 27);
//	em_printf("root>>> \"%s\".\n", buf);

	// System Initialization
    SystemCoreClockUpdate();
    // ...
    osKernelInitialize(); // Initialize CMSIS-RTOS
    osThreadNew(app_main, NULL, NULL); // Create application main thread
	osThreadNew(app_pwm, NULL, NULL); 
    osKernelStart(); // Start thread execution
    for (;;) {
    }

    return 0;
}

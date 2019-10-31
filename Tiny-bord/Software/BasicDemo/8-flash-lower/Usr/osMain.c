/*----------------------------------------------------------------------------
 * CMSIS-RTOS 'main' function template
 *---------------------------------------------------------------------------*/

#include "osMain.h"
#include "EventRecorder.h"  
#include "rl_fs.h"
/*----------------------------------------------------------------------------
 * Application main thread
 *---------------------------------------------------------------------------*/

osSemaphoreId_t semSerial;
SerialDev_t* emSerial;

uint16_t light = 0;
fsStatus s;



void app_main(void* argument)
{
	FILE *f;
	char * buf = emSerial->rbuf;
	char b[20];
	em_printf("root>>> Init flash.\n");
	uint8_t t1 = 0xFF, t2;
	
	uint32_t addr = 0;
	
	s = finit("F0:");
	s = fmount("F0:");

	//W25qxx_EraseChip();
	//fformat("F0:", "/W"); // take a long time 
//	extern uint32_t cnt;
//	em_printf("Erase:: %u times.\n", cnt);



	f = fopen ("hello.txt","w");
	fprintf (f, "helloworld\n");
	fclose(f);
	
	f = fopen ("log.txt","w");
	fprintf (f, "222222222222222222222222222222222t");
	fclose(f);	

	f = fopen ("test.txt","w");
	fprintf (f, "111111111111111111111111111111111t\n222222222222\nlllllllll\n");
	fclose(f);	
	//
	fsFileInfo info;
	
	
	em_printf("###############################################\n");
	while ( ffind("F0:*.*", &info) == fsOK )
	{
		em_printf("%-32s %5d bytes, ID: %04d\n",
		info.name,
		info.size,
		info.fileID);
	}
	
	f = fopen ("test.txt", "r");
	if ( f != NULL )
	{
		em_printf("root>>> vi test.txt\n");
		
		while ( fscanf (f, "%s", b) != EOF )
			em_printf("%s.\n", b);
		
		
	}
	
	em_printf("root>>> ");
	funmount("F0:");
	funinit("F0:");
	for (;;) {
//		W25qxx_Init();
		osSemaphoreAcquire(semSerial, osWaitForever);
		console_line_handle(buf, emSerial->rCnt);
		//em_printf("Thread>>> %s\n", buf);
		//sscanf(buf, "%d", &light);
		//em_printf("Thread>>> Light: %d.\n", light);
	}
}

void app_pwm(void* argument)
{
	uint16_t cnt = 100;
	uint16_t c = 50000;
	uint16_t cc;
    for (;;) {
//		sprintf(ss, "Flash Test: count %d", cnt ++);
//		Driver_FLASH.EraseSector(0x00);
//		Driver_FLASH.ProgramData(0x00, ss, strlen(ss));
//		Driver_FLASH.ReadData(0x00, bb, strlen(ss));
//		em_printf("flash>>> \"%s\".\n", bb);
		if ( light == 0 )
		{
			HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, GPIO_PIN_RESET);
			
			continue;
		}
		
		
		HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, GPIO_PIN_RESET);
		for ( int i = 0; i < 10000-light; i ++ )	{__NOP();};
		HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, GPIO_PIN_SET);
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
	
	console_cmd_init();

//    ARM_Flash_Initialize(NULL);
//    ARM_Flash_EraseSector(0xfff4);

//    ARM_Flash_ProgramData(0xfff4, str, strlen(str));
//    HAL_Delay(500);
//    em_printf("root>>>wait...\n");
//    ARM_Flash_ReadData(0xfff4, buf, 27);
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

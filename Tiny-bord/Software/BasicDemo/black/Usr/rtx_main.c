/*----------------------------------------------------------------------------
 * CMSIS-RTOS 'main' function template
 *---------------------------------------------------------------------------*/

#include "rtx_main.h"

#include "rl_fs.h"
/*----------------------------------------------------------------------------
 * Application main thread
 *---------------------------------------------------------------------------*/

mem_head_t * SRAMUsed;

osSemaphoreId_t semSerial;
SerialDev_t* emSerial;

extern GUI_CONST_STORAGE GUI_BITMAP bm0;
extern GUI_CONST_STORAGE GUI_BITMAP bm1;
extern GUI_CONST_STORAGE GUI_BITMAP bm2;
extern GUI_CONST_STORAGE GUI_BITMAP bm3;
extern GUI_CONST_STORAGE GUI_BITMAP bm4;
extern GUI_CONST_STORAGE GUI_BITMAP bm5;
extern GUI_CONST_STORAGE GUI_BITMAP bm6;
extern GUI_CONST_STORAGE GUI_BITMAP bm7;
extern GUI_CONST_STORAGE GUI_BITMAP bm8;
extern GUI_CONST_STORAGE GUI_BITMAP bm9;
extern GUI_CONST_STORAGE GUI_BITMAP bm10;
extern GUI_CONST_STORAGE GUI_BITMAP bm11;
extern GUI_CONST_STORAGE GUI_BITMAP bm12;




void app_main(void* argument)
{
	Init_GUIThread();
	while ( 1 )
	{
		//osDelay(1000);
		//HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
	
		//osSemaphoreAcquire(semSerial, osWaitForever);
		//em_printf("Get!\n");
		//console_line_handle(emSerial->rbuf, emSerial->rCnt);
		osDelay(50);
		GUI_DrawBitmap(&bm0, 0, 0);
		osDelay(50);
		GUI_DrawBitmap(&bm1, 0, 0);
		osDelay(50);		
		GUI_DrawBitmap(&bm2, 0, 0);
		osDelay(50);
		GUI_DrawBitmap(&bm3, 0, 0);
		osDelay(50);
		GUI_DrawBitmap(&bm4, 0, 0);
		osDelay(50);
		GUI_DrawBitmap(&bm5, 0, 0);
		osDelay(50);
		GUI_DrawBitmap(&bm6, 0, 0);
		osDelay(50);
		GUI_DrawBitmap(&bm7, 0, 0);
		osDelay(50);
		GUI_DrawBitmap(&bm8, 0, 0);
		osDelay(50);
		GUI_DrawBitmap(&bm9, 0, 0);
		osDelay(50);
		GUI_DrawBitmap(&bm10, 0, 0);
		osDelay(50);
		GUI_DrawBitmap(&bm11, 0, 0);
		osDelay(50);
		GUI_DrawBitmap(&bm12, 0, 0);

	}
}



void rtx_main(void)
{
	MallocInit();
	//EventRecorderInitialize (EventRecordAll, 1); 
	
    semSerial = osSemaphoreNew(1U, 0U, NULL);
    emSerial = registerSerial(&huart2, 512, 512, &semSerial);
    emSerial->Initialize(emSerial);
	//em_printf("root>>> start...\r\n");
	//em_printf("root>>> success.\r\n");
	//SRAMUsed = RAMINFO();
	//em_printf("root>>> ram used %d, total %d  :: %.2lf%%.\n", SRAMUsed->used, SRAMUsed->size, SRAMUsed->used*1.0/SRAMUsed->size);
	//console_cmd_init();
	//W25qxx_Init();

	OLED_Init();
	OLED_CLS();
	//OLED_DrawBMP(0,0,128,8,(unsigned char *)BMP1);
	//SRAMUsed = RAMINFO();
	//em_printf("root>>> ram used %d, total %d  :: %.2lf%%.\n", SRAMUsed->used, SRAMUsed->size, SRAMUsed->used*1.0/SRAMUsed->size);	
	FILE *f;
	if ( finit("F0:") != fsOK ) {
		em_printf("finit F0 failed.\n");
	}
	if ( fmount("F0:") != fsOK ) {
		em_printf("fmount F0 failed.\n");
	}	
//	em_printf("Open init.txt -a");
//	f = fopen("init.txt", "w");
//	fprintf(f, "This is Log Init");
//	fclose(f);
//	em_printf("write string.\nclose...");
	
	char str[20];
	f = fopen("init.txt", "r");
//	while ( fscanf(f, "%s", str) != EOF )
//	{
//		em_printf("FILE>>> %s.\n", str);
//	}
	fclose(f);
//	em_printf("read string.\nclose...\n");	
	
	
	console_cmd_init();
	
	// System Initialization
    SystemCoreClockUpdate();
    // ...
    osKernelInitialize(); // Initialize CMSIS-RTOS
    osThreadNew(app_main, NULL, NULL); // Create application main thread

    osKernelStart(); // Start thread execution
    for (;;) {
    }
}

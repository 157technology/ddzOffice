#include "startup.h"

#include <stdio.h>

#include "led.h"
#include "delay.h"
#include "ds18b20.h"

#include "rtos.h"

#include "GUI.h"
#include "w25qxx.h"
#include "usart.h"
#include "ui_print.h"

char str[20];

int main(void)
{
	//ledDrv.set(&ledDrv, 1, LED_ON);
	GUI_Init();
	GUI_SetColor(GUI_WHITE);
	GUI_SetPenSize(1);
	GUI_SetFont(GUI_FONT_8_ASCII);
	GUI_SetLBorder(0);
	W25qxx_Init();
	
	
//	GUI_DispStringAtCEOL("Tiny Bord", 0, 0);
//	GUI_DispNextLine();
//	//GUI_DispString("Tiny Bord");
//	ui_printf("root>>> ");
//	GUI_DispNextLine();
//	GUI_DispNextLine();
//	GUI_DispNextLine();
//	GUI_DispNextLine();
//	GUI_DispString("Copyright@2019-2029");
//	for ( int i = 0; i < 1000; i ++ )
//	{
//		sprintf(str, "Temper: %.2lf^C", Temper18b20.temper());
//		//HAL_UART_Transmit(&huart1, (uint8_t*)str, strlen(str), 999);
//		GUI_DispStringAtCEOL(str, 0, 0);
//		//GUI_DispCEOL();
//		HAL_Delay(50);		
//	}
	

	//GUI_Delay(1000);
	//GUI_DispStringAt("Hi!", 0, 0);
	
	ui_printf("start rtos...\n");
	rtosInit();
//	ledDrv.set(&ledDrv, 2, LED_OFF);
//	while ( 1 )
//	{
//		ledDrv.leftstream(&ledDrv, 80);
//		//ledDrv.set(&ledDrv, 1, LED_ON);
//		//HAL_Delay(10);
//		//Delay_us(10);
//		temper = Temper18b20.temper();
//		//ledDrv.set(&ledDrv, 1, LED_OFF);
//		//HAL_Delay(100);
//		Delay_us(50);
//	}
	
	/* never reach */
	return 0;
}
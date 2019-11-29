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

#include "serial.h"

#include "wifi.h"
#include "shell.h"

int main(void)
{
	//ledDrv.set(&ledDrv, 1, LED_ON);

	GUI_Init();
//	GUI_SetBkColor(GUI_WHITE);
//	GUI_SetColor(GUI_BLACK);
	GUI_SetPenSize(1);
	GUI_SetFont(GUI_FONT_8X8);
//	GUI_Clear();
	W25qxx_Init();
	GUI_Clear();
//	ui_printf("aaabbbccdddeeefffggg   vvvwwwwvvv");
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
	ui_printf("app_main start.\n");
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

uint16_t ledDemo(int argc, char *argv[])
{
	ledDrv.rightstream(100);
	ledDrv.rightstream(80);
	ledDrv.rightstream(60);
	ledDrv.leftstream(100);
	ledDrv.leftstream(80);
	ledDrv.leftstream(60);
	
	for ( int i = 0; i < ledDrv.num; i ++ )
		ledDrv.set(i+1, LED_OFF);
	for ( int c = 0; c < 10; c ++ )
	{
		for ( int i = 0; i < ledDrv.num; i ++ )
			ledDrv.toggle(i+1);
		osDelay(200);
	}
	return 0;
}
SHELL_CMD_REJESTER(ledDemo, led test);


uint16_t wifiTest(int argc, char *argv[])
{
	wifi.scanAP();

	return 0;
}
SHELL_CMD_REJESTER(wifiTest, wifi test);

uint16_t wifiJoin(int argc, char *argv[])
{
	wifi.joinAP("EMei_Li", "88888888");

	return 0;
}
SHELL_CMD_REJESTER(wifiJoin, wifi test);

uint16_t wifiQuit(int argc, char *argv[])
{
	wifi.quitAP();

	return 0;
}
SHELL_CMD_REJESTER(wifiQuit, wifi test);

uint16_t ipconfig(int argc, char *argv[])
{
	wifi.ipconfig();

	return 0;
}
SHELL_CMD_REJESTER(ipconfig, wifi test);

uint16_t ping(int argc, char *argv[])
{
	wifi.ping("www.baidu.com");

	return 0;
}
SHELL_CMD_REJESTER(ping, wifi test);

uint16_t dns(int argc, char *argv[])
{
	wifi.dns("www.baidu.com");

	return 0;
}
SHELL_CMD_REJESTER(dns, wifi test);

uint16_t tcp(int argc, char *argv[])
{
	wifi.tcp("101.200.53.190", 80);

	return 0;
}
SHELL_CMD_REJESTER(tcp, wifi test);

uint16_t udp(int argc, char *argv[])
{
	wifi.udp("192.168.43.74", 8888);

	return 0;
}
SHELL_CMD_REJESTER(udp, wifi test);

uint16_t send(int argc, char *argv[])
{
	wifi.send("get\r\n");

	return 0;
}
SHELL_CMD_REJESTER(send, wifi test);


uint16_t fsm(int argc, char *argv[])
{
	wifi.fsm();

	return 0;
}
SHELL_CMD_REJESTER(fsm, wifi test);

#include "main.h"
#include "GUI.h"

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

#include "cmsis_os2.h"
#include "oled.h"

static char ui_buf[512];
static GUI_RECT rect = {0, 0, 127, 63}; 
uint16_t x, y, lock;

void ui_gui_reset()
{
	GUI_Clear();
	x = 0;
	y = 0;
	lock = 0;
}

void ui_printf(const char *format, ...)
{
	while ( lock == 1 )
	{
		osDelay(50);
	}		
	lock = 1;
    va_list args;
    va_start(args, format);


	uint16_t length;
	


    length = vsnprintf(ui_buf, 512, (char*)format, args);
    va_end(args);
 	//x = GUI_GetDispPosX();
	//y = GUI_GetDispPosY();  
	if ( x != 0 )
	{
		GUI_DispNextLine();
		y = GUI_GetDispPosY();
		
		rect.x0 = 0;	rect.y0 = y;
	}
	else
	{
		rect.x0 = 0;	rect.y0 = y;
	}
    GUI_DispStringInRectWrap(ui_buf, &rect, GUI_TA_LEFT, GUI_WRAPMODE_CHAR);
	//GUI_DispString("hello");

	GUI_DispCEOL();

//	//GUI_DispCEOL();
//	x = GUI_GetDispPosX();
//	y = GUI_GetDispPosY();	
	GUI_DispNextLine();
	GUI_DispCEOL();
//	uint16_t yn = GUI_GetDispPosY();
//	if ( yn > 63 )
//	{
//		y = 0;
//		GUI_GotoX(0);
//	}
//	else
//		GUI_GotoX(x);
//	
//	GUI_GotoY(y);
	y += 8;
	if ( y + 8 > 64 )	
	{
		OLED_Replot();
		HAL_Delay(800);
		OLED_Fill(0x00);
		y = 0;
	}
	lock = 0;
}
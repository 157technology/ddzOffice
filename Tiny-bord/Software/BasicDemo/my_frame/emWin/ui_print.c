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

	if ( rect.y0 == 0 )
		GUI_Clear();
	
	int lines = GUI_WrapGetNumLines(ui_buf, 128, GUI_WRAPMODE_CHAR);
    GUI_DispStringInRectWrap(ui_buf, &rect, GUI_TA_LEFT, GUI_WRAPMODE_CHAR);
	GUI_DispCEOL();

	x = GUI_GetDispPosX();
	y = GUI_GetDispPosY();	
//	GUI_DispDec(x, 3);
//	GUI_DispDec(y, 3);

//	GUI_DispDec(lines, 3);
	GUI_DispNextLine();
//	GUI_DispCEOL();
	uint16_t yn = GUI_GetDispPosY();
	if ( yn > 63 )
	{
		rect.y0 = 0;
		y = 0;
	}
	else
		rect.y0 = yn;

	lock = 0;
}
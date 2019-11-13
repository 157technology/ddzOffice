#include "main.h"
#include "GUI.h"

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

static char ui_buf[512];
static GUI_RECT rect = {0, 0, 127, 63}; 
uint16_t x, y;
void ui_printf(const char *format, ...)
{
    va_list args;
    va_start(args, format);


	uint16_t length;
	


    length = vsnprintf(ui_buf, 512, (char*)format, args);
    va_end(args);
	//emHAL_UART_Transmit_DMA(m_consoleDev->puart, (uint8_t*)(m_consoleDev->tbuf), length);
 	x = GUI_GetDispPosX();
	y = GUI_GetDispPosY();  
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
	GUI_DispCEOL();
	
	x = GUI_GetDispPosX();
	y = GUI_GetDispPosY();
	
	GUI_DispNextLine();
	GUI_DispCEOL();
	uint16_t yn = GUI_GetDispPosY();
	if ( yn > 63 )
	{
		y = 0;
		GUI_GotoX(0);
	}
	else
		GUI_GotoX(x);
	
	GUI_GotoY(y);
	
	
	
}
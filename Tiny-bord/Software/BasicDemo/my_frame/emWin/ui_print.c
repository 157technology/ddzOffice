#include "main.h"
#include "GUI.h"

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

static char ui_buf[512];
void ui_printf(const char *format, ...)
{
    va_list args;
    va_start(args, format);


	uint16_t length, x, y;
	


    length = vsnprintf(ui_buf, 512, (char*)format, args);
    va_end(args);
	//emHAL_UART_Transmit_DMA(m_consoleDev->puart, (uint8_t*)(m_consoleDev->tbuf), length);
    GUI_DispString(ui_buf);
	GUI_DispCEOL();
	x = GUI_GetDispPosX();
	y = GUI_GetDispPosY();
	
	/**/
	if ( y > 60 )
	{
		GUI_GotoXY(0, 0);
	}		
	
}
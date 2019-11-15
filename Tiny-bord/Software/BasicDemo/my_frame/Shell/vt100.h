#ifndef __VT100_H__
#define __VT100_H__ 

#include "serial.h"
//
//
//VT100 控制码

#define CONSOLE_RESET		"\033[0m"
#define CONSOLE_HIGHLIGHT	"\033[1m"
#define CONSOLE_UNDERLINE	"\033[4m"
#define CONSOLE_BLINK		"\033[5m"
#define CONSOLE_REVERSE		"\033[7m"

#define CONSOLE_UP			"\033[%dA"
#define CONSOLE_DOWN		"\033[%dB"
#define CONSOLE_RIGHT		"\033[%dC"
#define CONSOLE_LEFT		"\033[%dD"

#define CONSOLE_SET(x, y)	"\033["#y";"#x"H"
#define CONSOLE_CLEAR		"\033[2J"
#define CONSOLE_CL			"\033[K"
#define CONSOLE_SAVE		"\033[s"
#define CONSOLE_RESTORE		"\033[u"


#define CONSOLE_BLACK		"\033[30m"
#define CONSOLE_RED			"\033[31m"
#define CONSOLE_GREEN		"\033[32m"
#define CONSOLE_YELLOW		"\033[33m"
#define CONSOLE_BLUE		"\033[34m"
#define CONSOLE_PURPLE		"\033[35m"
#define CONSOLE_CYAN		"\033[36m"
#define CONSOLE_WHITE		"\033[37m"

#define CONSOLE_BG_BLACK	"\033[40m"
#define CONSOLE_BG_RED		"\033[41m"
#define CONSOLE_BG_GREEN	"\033[42m"
#define CONSOLE_BG_YELLOW	"\033[43m"
#define CONSOLE_BG_BLUE		"\033[44m"
#define CONSOLE_BG_PURPLE	"\033[45m"
#define CONSOLE_BG_CYAN		"\033[46m"
#define CONSOLE_BG_WHITE	"\033[47m"


#define __set_cursor(cmd, n)		em_printf(cmd, n)

#define __set_console(cmd)			em_printf(cmd)

#endif
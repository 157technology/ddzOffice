#include "shell.h"
#include "key_code.h"
#include "vt100.h"
#include "shell_config.h"
#include "shell_string.h"
#include "shell_history.h"
#include "shell_cmd.h"
#include "ui_print.h"

#ifdef SHELL_DEBUG
	#include "serial.h"
	#define SHELL_LOG	em_printf
#endif



shell_t shell = {
	SHELL_CONSOLE
};

/**/
static void shell_handle_console(char * str);


int shell_init()
{
	if ( !shell_string_init() )		return 0;
	if ( !shell_history_init() )	return 0;

	em_printf("\n");
	em_printf("****   ****   ******		Core: 	STM32F411\n");
	em_printf("*   *  *   *     **		RTOS: 	RTX5\n");
	em_printf("*   *  *   *    ** 		FILE: 	RT-FILE\n");
	em_printf("*   *  *   *   **		GRAP:	EM-WIN\n");
	em_printf("****   ****   ******		SCREEM:	OLED-128*64\n");
	em_printf("====================		1.0.0 Build 2019-11-6 by EMei_Li\n");
//	em_printf("2019 - 2029 Copyright by DDZ team\n");
	em_printf("root>>> ");	
	ui_gui_reset();
//	ui_printf("-###----###---######-");
//	ui_printf("-####---####-----##--");
//	ui_printf("-####---####----##---");
//	ui_printf("-####---####---##----");
//	ui_printf("-###----###---######-");
	ui_printf("root>>> ");
	return 1;
}

void shell_handle(char * str)
{
	__set_console(CONSOLE_BLUE);
	if ( shell.mode == SHELL_CONSOLE )
	{
		shell_handle_console(str);
	}
	else
	{
		#ifdef SHELL_DEBUG
		SHELL_LOG("Shell Subsys.\n");
		#endif		
	}
	__set_console(CONSOLE_WHITE);
}


static void shell_handle_console(char * str)
{
	__set_console(CONSOLE_BLUE);

	if ( __isKey(str, KEY_UP) )
	{
		#ifdef SHELL_DEBUG
		//SHELL_LOG("press Key up.\n");
		//SHELL_LOG(shellHistory.prev());
		#endif
		char * his = shellHistory.prev();
		if ( his != NULL )
		{
			shellString.set(his);
			__set_console(CONSOLE_BLUE);
			em_printf(his);
		}
	}
	else if ( __isKey(str, KEY_DOWN) )
	{
		#ifdef SHELL_DEBUG
		//SHELL_LOG("press Key Down.\n");
		//SHELL_LOG(shellHistory.next());
		#endif
		char * his = shellHistory.next();
		if ( his == NULL )
		{
			shellString.set("");
			//em_printf("");
		}
		else
		{
			shellString.set(his);
			__set_console(CONSOLE_GREEN);
			em_printf(his);			
		}
	}
	else if ( __isKey(str, KEY_LEFT) )
	{
		#ifdef SHELL_DEBUG
		//SHELL_LOG("press Key Left.\n");
		#endif
		shellString.cursor_left(1);
		//em_printf("\033[1D");
	}
	else if ( __isKey(str, KEY_RIGHT) )
	{
		#ifdef SHELL_DEBUG
		//SHELL_LOG("press Key Right.\n");
		#endif
		shellString.cursor_right(1);
		//em_printf("\033[1C");
	}
	
	
//////////////////////////////////////////////////////////	
	
	
	else if ( __isKey(str, KEY_TAP) )
	{
		#ifdef SHELL_DEBUG
		//SHELL_LOG("press Key Tap.\n");
		#endif
		
		shellString.autoComplete();
	}
	else if ( __isKey(str, KEY_Ctrl_S) )
	{
		#ifdef SHELL_DEBUG
		SHELL_LOG("press Key Ctrl S.\n");
		#endif
	}
	else if ( __isKey(str, KEY_Ctrl_C) )
	{
		#ifdef SHELL_DEBUG
		SHELL_LOG("press Key Ctrl_C.\n");
		#endif
	}
	
//////////////////////////////////////////////////////////	
	
	else if ( __isKey(str, KEY_ENTER) )
	{
		#ifdef SHELL_DEBUG
		//SHELL_LOG("press Enter.\n");
		#endif
		shellHistory.insert(shellString.str);
		ui_printf("%s\n", shellString.str);
		__set_console(CONSOLE_RED);
		shell_cmd_handle(shellString.str);
		shellString.reset();

		if ( shell.mode == SHELL_CONSOLE )
		{
			__set_console(CONSOLE_WHITE);
			em_printf("\nroot>>> ");
		}
	}
	else if ( __isKey(str, KEY_BACKSPACE) )
	{
		#ifdef SHELL_DEBUG
		//SHELL_LOG("press Backspace.\n");
		#endif		
		shellString.cursor_backspace(1);
	}
	else
	{
		#ifdef SHELL_DEBUG
		//SHELL_LOG("press Key.\n");
		#endif
		em_printf("%s", str);
		shellString.insert(str);
	}
	__set_console(CONSOLE_WHITE);
}



uint16_t help(int argc, char *argv[])
{
	em_printf("%8s | %32s |\n", "cmd", "descrition");
	em_printf("%8s | %32s |\n", "--------", "--------------------------------");
	for ( shell_cmd_t ** ppcmd = (shell_cmd_t **)&ShellCmdTab$$Base; ppcmd != (shell_cmd_t **)&ShellCmdTab$$Limit; ppcmd ++ )
	{
		shell_cmd_t * pcmd = *ppcmd;
		em_printf("%8s | %32s |\n", pcmd->cmd, pcmd->help);
	}	
	return 0;
}
SHELL_CMD_REJESTER(help, show all command);

uint16_t test(int argc, char *argv[])
{
	//__setShellMode(SHELL_SUBSYS);
	em_printf("*****************\n");
	em_printf("Test Cmd\n");
	em_printf("*****************\n");
	return 1;
}
SHELL_CMD_REJESTER(test, testhelp);

uint16_t test1(int argc, char *argv[])
{
	em_printf("*****************\n");
	em_printf("Test1 Cmd\n");
	em_printf("*****************\n");
	return 1;
}
SHELL_CMD_REJESTER(test1, test1help);



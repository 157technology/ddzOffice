#include "shell_string.h"
#include "shell_config.h"
#include "shell_keyWord.h"
#include "string.h"
#include "memory.h"
#include "vt100.h"

#ifdef SHELL_DEBUG
#include "serial.h"
#define SHELL_LOG	em_printf
#endif

shell_string_t shellString;


/*######################################*/
static void shell_string_reset();
static void shell_string_insert(char * str);
static void shell_string_setString(char * str);
static void shell_string_cursor_left(uint16_t step);
static void shell_string_cursor_right(uint16_t step);
static void shell_string_cursor_backspace(uint16_t step);
static void shell_string_autoComplete();

/*######################################*/

static void shell_string_reset()
{
	shellString.cursor = 0;
	shellString.len 	= 0;

	memset(shellString.str, '\0', SHELL_STRING_MAX_LEN);
}

static void shell_string_insert(char * str)
{
	if ( shellString.cursor == shellString.len )
	{
		// 直接插入
		for ( int i = 0; i < strlen(str); i ++ )
		{
			shellString.str[shellString.cursor] = str[i];
			shellString.cursor ++;
			shellString.len ++;
		}
	}
	else if ( shellString.cursor < shellString.len )
	{
		uint16_t posend 	= shellString.len - 1;
		uint16_t posbegin	= shellString.cursor;
		uint16_t len 		= strlen(str);
		for ( int i = posend; i >= posbegin; i -- )
		{
			shellString.str[i + len] = shellString.str[i];
		}
		for ( int i = 0; i < len; i ++ )
		{
			shellString.str[posbegin + i] = str[i];
		}
		shellString.cursor += len;
		shellString.len    += len;
	}
	else
	{

	}
#ifdef SHELL_DEBUG
	__set_console(CONSOLE_RED);
	//SHELL_LOG("\n%2d -> %s\n", shellString.len, shellString.str);
	__set_console(CONSOLE_WHITE);
#endif
}

static void shell_string_setString(char * str)
{
	shell_string_cursor_right(shellString.len-shellString.cursor);
	shell_string_cursor_backspace(shellString.len);

	memcpy(shellString.str, str, strlen(str)+1);
	shellString.len = strlen(str);
	shellString.cursor = shellString.len;
}

static void shell_string_cursor_left(uint16_t step)
{
	uint16_t valid_step = step > shellString.cursor ? shellString.cursor : step;

	shellString.cursor -= valid_step;
	__set_cursor(CONSOLE_LEFT, valid_step);
}

static void shell_string_cursor_right(uint16_t step)
{
	uint16_t valid_step = step > shellString.len - shellString.cursor ? shellString.len - shellString.cursor : step;

	shellString.cursor += valid_step;
	__set_cursor(CONSOLE_RIGHT, valid_step);
}

static void shell_string_cursor_backspace(uint16_t step)
{
	if ( shellString.cursor == shellString.len )
	{
		for ( int i = 0; i < step; i ++ )
		{
			if ( shellString.cursor == 0 )
				break;

			shellString.cursor --;
			shellString.len --;
			shellString.str[shellString.cursor] = '\0';

			__set_console(BACKSPACE);
		}
	}
	else
	{
		for ( int i = 0; i < step; i ++ )
		{
			if ( shellString.cursor == 0 )
				break;

			for ( int pos = shellString.cursor - 1; pos < shellString.len - 1; pos ++ )
			{
				shellString.str[pos] = shellString.str[pos + 1];
			}
			shellString.cursor --;
			shellString.len --;
			shellString.str[shellString.len] = '\0';

			__set_console(BACKSPACE);
		}

	}
#ifdef SHELL_DEBUG
	__set_console(CONSOLE_BLUE);
	//SHELL_LOG("\n%2d -> %s\n", shellString.len, shellString.str);
	__set_console(CONSOLE_WHITE);
#endif
}

static void shell_string_autoComplete()
{
	if ( shellString.len < 2 )
		return;

	for ( int i = 0; i < 3; i ++ )
	{
		if ( strncmp(shellString.str, shellKeyWord[i], strlen(shellString.str)) == 0 )
		{
			for ( int j = 0; j < shellString.cursor; j ++ )	__set_console(BACKSPACE);
			SHELL_LOG(shellKeyWord[i]);
			strcpy(shellString.str, shellKeyWord[i]);
			shellString.len = strlen(shellKeyWord[i]);
			shellString.cursor = shellString.len;
		}
	}
}

int shell_string_init()
{
	shellString.str = MALLOC(sizeof(char) * SHELL_STRING_MAX_LEN);
	if ( shellString.str == NULL )
	{
		return 0;
	}
	shellString.len = 0;
	shellString.cursor = 0;
	shellString.cursor_left 	 = shell_string_cursor_left;
	shellString.cursor_right	 = shell_string_cursor_right;
	shellString.cursor_backspace = shell_string_cursor_backspace;
	shellString.insert			 = shell_string_insert;
	shellString.set 			 = shell_string_setString;
	shellString.autoComplete	 = shell_string_autoComplete;
	shellString.reset			 = shell_string_reset;
	return 1;
}
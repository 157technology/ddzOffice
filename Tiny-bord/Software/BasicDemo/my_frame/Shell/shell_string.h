#ifndef __SHELL_STRING_H__
#define __SHELL_STRING_H__


#include "main.h"


#define BACKSPACE			"\x7F"




typedef struct shell_string_s
{
	char * str;

	uint16_t len;
	uint16_t cursor;

	void (* cursor_left)		(uint16_t step);
	void (* cursor_right)		(uint16_t step);
	void (* cursor_backspace)	(uint16_t step);

	void (* insert)				(char * str);

	void (* autoComplete)		(void);

	void (* reset)				(void);
}shell_string_t;


int shell_string_init();
extern shell_string_t shellString;

#endif
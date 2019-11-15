#ifndef __SHELL_CONFIG_H__
#define __SHELL_CONFIG_H__ 

#include "serial.h"

#define SHELL_DEBUG


#define SHELL_KEYWORD_MAX_LEN	32

#define SHELL_STRING_MAX_LEN	(8*128)	// string

#define SHELL_MAX_LEN			128 	// cmd

/*
	历史记录
 */
#define SHELL_HISTORY_AMOUNT	10 		// history

#endif
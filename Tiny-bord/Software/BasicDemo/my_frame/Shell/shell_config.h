#ifndef __SHELL_CONFIG_H__
#define __SHELL_CONFIG_H__ 

#include "serial.h"
#include <string.h>
#include "main.h"

#define SHELL_DEBUG

typedef enum 
{
	SHELL_CONSOLE = 0,
	SHELL_SUBSYS
}Shell_Mode;



typedef struct shell_s
{
	Shell_Mode mode;
	//初始化
	//光标操作
	//历史记录
	//自动补全
	//
}shell_t;

extern shell_t shell;

/**/
#define __setShellMode(m)		shell.mode = m

#define __isKey(str, key) 		!strcmp((str), (key))

#define SHELL_CMDS_AMOUNT		8       //
#define SHELL_CMDS_MAX_LEN		32

#define SHELL_STRING_MAX_LEN	(8*128)	// string

#define SHELL_MAX_LINE			128 	// cmd

/*
	历史记录
 */
#define SHELL_HISTORY_AMOUNT	10 		// history


#define SHELL_SKIP_SPACE(str)	while(*(str) == ' ') str++





extern const int ShellCmdTab$$Base;
extern const int ShellCmdTab$$Limit;


#endif
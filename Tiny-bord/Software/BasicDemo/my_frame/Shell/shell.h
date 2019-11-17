#ifndef __SHELL_H__
#define __SHELL_H__ 

#include "main.h"
#include <string.h>

#define __isKey(str, key) 	!strcmp((str), (key))

#define SECTION(x)                  __attribute__((section(x)))


typedef struct shell_cmd_s
{
	const char * cmd;
	const char * help;
	uint16_t (* run) 	(int argc, char *argv[]);
}shell_cmd_t;


extern const int ShellCmdTab$$Base;
extern const int ShellCmdTab$$Limit;
#define SHELL_CMD_REJESTER(name, help, fun)			\
	const char __shell_cmd_##name##_name[] SECTION(".rodata.name") = #name;		\
	const char __shell_cmd_##name##_desc[] SECTION(".rodata.name") = #help;		\
	const shell_cmd_t __shell_cmd_##name   SECTION(".rodata.cmd")  = 			\
	{																			\
		__shell_cmd_##name##_name,												\
		__shell_cmd_##name##_desc,												\
		fun,																	\
	};																			\
	const shell_cmd_t * __shell_cmd_##name##_ptr   SECTION("ShellCmdTab")  =    \
		&__shell_cmd_##name;													


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


int shell_init();
void shell_handle(char * str);

void cmdtest(void);

#endif
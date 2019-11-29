#ifndef __SHELL_H__
#define __SHELL_H__ 

#include "main.h"

typedef struct shell_cmd_s
{
	const char * cmd;
	const char * help;
	uint16_t (* run) 	(int argc, char *argv[]);
}shell_cmd_t;

#define SECTION(x)				__attribute__((section(x)))

#define SHELL_CMD_REJESTER(name, desc)			\
	const char __shell_cmd_##name##_name[] SECTION(".rodata.name") = #name;		\
	const char __shell_cmd_##name##_desc[] SECTION(".rodata.name") = #desc;		\
	const shell_cmd_t __shell_cmd_##name   SECTION(".rodata.cmd")  = 			\
	{																			\
		__shell_cmd_##name##_name,												\
		__shell_cmd_##name##_desc,												\
		name,																	\
	};																			\
	const shell_cmd_t * __shell_cmd_##name##_ptr   SECTION("ShellCmdTab")  =    \
		&__shell_cmd_##name;

int shell_init();
void shell_handle(char * str);


#endif
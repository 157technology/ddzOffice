#ifndef __SHELL_H__
#define __SHELL_H__ 

#include <string.h>

#define __isKey(str, key) 	!strcmp((str), (key))


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

#endif
#ifndef __SHELL_H__
#define __SHELL_H__ 

#include <shell_config.h>
#include <serial.h>

#include <stdio.h>
#include <string.h>

typedef struct tHistoryStack
{
	char cmd_history[10][50];
	uint8_t root;
	uint8_t top;
	uint8_t cur;
	uint8_t is_Empty;

	void 		(* init)		(struct tHistoryStack * this);
	void 		(* append)		(struct tHistoryStack * this, char * str);
	char * 		(* previous)	(struct tHistoryStack * this);
	char * 		(* next)	    (struct tHistoryStack * this);
}historyStack_t;


typedef struct tConsoleString
{
	char str[128];
	uint16_t len;		//string 长度
	uint16_t cursor;	//光标位置

	void   (*setString)		(struct tConsoleString * this, char* str);
	void   (*reset)			(struct tConsoleString * this);
	void   (*insert)		(struct tConsoleString * this, char c);
	void   (*remove)		(struct tConsoleString * this);
	char * (*getString)		(struct tConsoleString * this);

}consoleString_t;


typedef struct tConsoleCmd
{
	char name[50];
	char help[50];

	void (*run) (int argc, char const *argv[]);

	struct tConsoleCmd * pnextcmd;
}consoleCmd_t;



typedef struct tConsoleCmdManage
{
	consoleCmd_t * root;
	consoleCmd_t * top;

	uint16_t count;

	void 			(*append)	(struct tConsoleCmdManage * this, tConsoleCmd * pcmd);
	consoleCmd_t *	(*find)		(struct tConsoleCmdManage * this, char * str);

}consoleCmdManage_t;

void shell_line_handle(char * str, uint16_t len);


consoleCmd_t rootCmd;


//#define CMD_ADD(name, help, fun) 	static const consoleCmd_t s_cmd_##name = \
//									{name, help, fun, rootCmd}


#endif
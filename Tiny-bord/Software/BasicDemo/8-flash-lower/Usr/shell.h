#ifndef __SHELL_H__
#define __SHELL_H__ 

#include <shell_config.h>
#include <serial.h>

#include <stdio.h>
#include <string.h>

typedef struct tHistoryStack
{
	char cmd_history[10][20];
	uint8_t root;
	uint8_t top;
	uint8_t cur;
	uint8_t is_Empty;

	void 		(* init)		(struct tHistoryStack * this);
	void 		(* append)		(struct tHistoryStack * this, char * str);
	char * 		(* previous)	(struct tHistoryStack * this);
	char * 		(* next)	    (struct tHistoryStack * this);
}historyStack_t;

void shell_line_handle(char * str, uint16_t len);

#endif
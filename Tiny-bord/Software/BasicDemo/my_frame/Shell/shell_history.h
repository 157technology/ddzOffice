#ifndef __SHELL_HISTORY_H__
#define __SHELL_HISTORY_H__ 

#include "shell_config.h"

#include <string.h>
#include "main.h"

typedef struct m_shell_history_s
{
	char (*buf)[SHELL_MAX_LINE];
	
	uint16_t root;
	uint16_t head;
	uint16_t pos;

	void	 	(*insert)		(char * str);
	char * 		(*next)			(void);
	char * 		(*prev)			(void); 

}shell_history_t;

extern shell_history_t shellHistory;
int shell_history_init();

#endif
#include "shell_history.h"

#include "vt100.h"

#include "memory.h"

#ifdef SHELL_DEBUG
	#include "serial.h"
	#define SHELL_LOG	em_printf
#endif



shell_history_t shellHistory;

static uint16_t next_index(uint16_t index)
{
	if ( index + 1 == SHELL_HISTORY_AMOUNT )	return 0;
	else										return index+1;
}
static uint16_t prev_index(uint16_t index)
{
	if ( index  == 0 )	return SHELL_HISTORY_AMOUNT - 1;
	else				return index-1;
}

static void shell_history_insert(char * str)
{
	for (int i = shellHistory.root; i != shellHistory.head; i = next_index(i))
	{
		/* code */
		if ( strcmp(str, shellHistory.buf[i]) == 0 )
		{
			for (int j = next_index(i); j != shellHistory.head; j = next_index(j))
			{
				/* code */
				int des = prev_index(j);

				memcpy( shellHistory.buf[des], shellHistory.buf[j], strlen(shellHistory.buf[j])+1 );
			}
			memcpy( shellHistory.buf[prev_index(shellHistory.head)], str, strlen(str)+1 );
			shellHistory.pos = shellHistory.head;
			return;
		}
	}
	memcpy( shellHistory.buf[shellHistory.head], str, strlen(str)+1 );
	shellHistory.head = next_index(shellHistory.head);

	if ( shellHistory.head == shellHistory.root )
	{
		shellHistory.root = next_index(shellHistory.root);
	}

	shellHistory.pos = shellHistory.head;
}


static char * shell_history_next()
{
	if ( shellHistory.head == shellHistory.pos )
	{
		return NULL;
	}
	shellHistory.pos = next_index(shellHistory.pos);
	if ( shellHistory.pos == shellHistory.head )	return NULL;
	
	return shellHistory.buf[shellHistory.pos];

}

static char * shell_history_prev()
{
	if ( shellHistory.root == shellHistory.head )
	{
		return NULL;
	}
	uint16_t pos = shellHistory.pos;
	if ( shellHistory.root != shellHistory.pos )
	{
		pos = prev_index(pos);
	}

	shellHistory.pos = pos;

	return shellHistory.buf[pos];
}



/*
	shell history init
 */
int shell_history_init()
{
	//初始化内存
	//
	shellHistory.buf = MALLOC(sizeof(char)*SHELL_MAX_LINE*SHELL_HISTORY_AMOUNT);
	if ( shellHistory.buf == NULL )
	{
		#ifdef SHELL_DEBUG
		SHELL_LOG("Malloc memory for shell history failed");
		#endif
		return 0;
	}
	shellHistory.pos  = 0;
	shellHistory.root = 0;
	shellHistory.head = 0;
	shellHistory.insert = shell_history_insert;
	shellHistory.next 	= shell_history_next;
	shellHistory.prev 	= shell_history_prev;
	//test
	#ifdef SHELL_DEBUG

	shellHistory.insert("This is Test begin");
	shellHistory.insert("Second");
	shellHistory.insert("333");
	shellHistory.insert("This is Test begin");
	shellHistory.insert("444");
	shellHistory.insert("555");
	shellHistory.insert("666");
	shellHistory.insert("777");

	shellHistory.insert("888");
	shellHistory.insert("999");
	shellHistory.insert("000");
	
	__set_console(CONSOLE_GREEN);
	for ( int i = shellHistory.root; i != shellHistory.head; i = next_index(i) )
	{
		SHELL_LOG("shell history>>> %5d-> %20s\n", i, shellHistory.buf[i]);
	}

	
	__set_console(CONSOLE_RED);
	SHELL_LOG("shell history>>> next :: %s\n", shellHistory.next()==NULL?"NULL":"HEI");
	SHELL_LOG("shell history>>> prev :: %s\n", shellHistory.prev());
	SHELL_LOG("shell history>>> next :: %s\n", shellHistory.next());
	SHELL_LOG("shell history>>> prev :: %s\n", shellHistory.prev());
	SHELL_LOG("shell history>>> next :: %s\n", shellHistory.next());
	SHELL_LOG("shell history>>> next :: %s\n", shellHistory.next());
	SHELL_LOG("shell history>>> next :: %s\n", shellHistory.next());
	SHELL_LOG("shell history>>> next :: %s\n", shellHistory.next());
	SHELL_LOG("shell history>>> next :: %s\n", shellHistory.next());
	SHELL_LOG("shell history>>> next :: %s\n", shellHistory.next());
	SHELL_LOG("shell history>>> prev :: %s\n", shellHistory.prev());
	SHELL_LOG("shell history>>> next :: %s\n", shellHistory.next()==NULL?"NULL":"HEI");

	__set_console(CONSOLE_WHITE);
	#endif
	
	return 1;
}
#include "shell.h"






/////////////////////////////////////////////////////////////////
// cmd history
// 实现--环形双向链表
static void hInitialise(historyStack_t * this)
{
	this->root = 0;
	this->top  = 0;
	this->cur  = 0;
	this->is_Empty = 0;
}

static void hAppend(historyStack_t * this, char * str)
{

	if ( this->is_Empty == 0 )
	{
		memcpy(this->cmd_history[this->top], str, strlen(str));
		this->top ++;
		this->is_Empty = 1;
	}
	else
	{
		if ( strcmp(this->cmd_history[this->top == 0? 9:this->top-1], str) != 0 )
		{
			memcpy(this->cmd_history[this->top], str, strlen(str));
			this->top ++;
			if ( this->top == 10 )	this->top = 0;

			if ( this->top == this->root )
			{
				this->root ++;
				if ( this->root == 10 )	this->root = 0;
			}
		}
	}
	this->cur = this->top;
}

static char * hPrevious(historyStack_t * this)
{
	if ( this->is_Empty == 0 )
		return NULL;

	if ( this->cur == this->root )
	{
		return this->cmd_history[this->root];
	}
	else
	{
		if ( this->cur == 0 )	this->cur = 9;
		else 					this->cur --;

		return this->cmd_history[this->cur];
	}
}

static char * hNext(historyStack_t * this)
{
	if ( this->cur == this->top )
		return NULL;
	else
	{
		if ( this->cur == 9 )	this->cur = 0;
		else					this->cur ++;
		if ( this->cur == this->top ) return NULL;
		return this->cmd_history[this->cur];
	}
}

// 注册到结构体中
historyStack_t emHistory = {
	{"", "", "", "", "", "", "", "", "", ""},
	0,
	0,
	0,
	0,
	hInitialise,//hInitialise,
	hAppend,//hAppend,
	hPrevious,
	hNext
};
/////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////
// 字符串输入处理
// 插入, 删除
//	void   (*setString)		(struct tConsoleString * this, char* str);
//	void   (*reset)			(struct tConsoleString * this);
//	void   (*insert)		(struct tConsoleString * this, char c);
//	void   (*remove)		(struct tConsoleString * this);
//	char * (*getString)		(struct tConsoleString * this);
static void consoleStringSetString(consoleString_t * this, char* str)
{
	uint16_t len = strlen(str);
	for ( int i = this->cursor; i < this->len; i ++ )
	{
		em_printf("\033[C");
	}
	for ( int i = 0; i < this->len; i ++ )
		em_printf("%c", KEY_BACKSPACE);

	if ( str == NULL )
	{
		this->len = 0;
		this->cursor = 0;
		return;
	}
	memcpy(this->str, str, len);
	this->len    = len;
	this->cursor = len; 


	em_printf("%s", str);
}

static void consoleStringReset(consoleString_t * this)
{
	this->cursor = 0;
	this->len 	 = 0;
}

static void consoleStringInsert(consoleString_t * this, char c)
{
	if ( this->cursor < this->len )
	{
		for ( int i = this->len; i > this->cursor; i -- )
		{
			this->str[i] = this->str[i - 1];
		}
		this->str[this->cursor] = c;
		this->cursor ++;
		this->len ++;
	}
	else if ( this->cursor == this->len )
	{
		this->str[this->cursor] = c;

		this->cursor ++;
		this->len ++;
	}
	else
	{
		em_printf("Insert Error: cursor too large.\n");
	}
}

static void consoleStringRemove(consoleString_t * this)
{
	if ( this->cursor < this->len )
	{
		if ( this->cursor != 0 )
		{
			for ( int i = this->cursor - 1; i < this->len; i ++ )
			{
				this->str[i] = this->str[i + 1];
			}
			this->cursor --;
			this->len --;
		}
	}
	else if ( this->cursor == this->len )
	{
		this->cursor --;
		this->len --;
	}
	else
	{
		em_printf("Insert Error: cursor too large.\n");
	}
}
static char * consoleStringGetString(consoleString_t * this)
{
	this->str[this->len] = '\0';
	return this->str;
}


consoleString_t emConsoleString =  {
	"",
	0,
	0,
	consoleStringSetString,
	consoleStringReset,
	consoleStringInsert,
	consoleStringRemove,
	consoleStringGetString,
};
/////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////
///     CMD
///
void consoleCmdManageAppend(consoleCmdManage_t * this, consoleCmd_t * pcmd)
{
	if ( this->count == 0 )
	{
		
	}
	this->top->pnextcmd = pcmd;
	this->top = pcmd;
	this->count ++;
}

consoleCmd_t * consoleCmdManageFind(consoleCmdManage_t * this, char * str)
{
	if ( this->count == 0 )	return NULL;
	consoleCmd_t * pcmd = this->root.pnextcmd;
	for ( int i = 0; i < this->count; i ++ )
	{
		if ( strcmp(pcmd->name, str) == 0 )
		{
			return pcmd;
		}
		pcmd = pcmd->pnextcmd;
	}
	return NULL;
}


/////////////////////////////////////////////////////////////////

static uint8_t m_deal_special_button(char * str, uint16_t len)
{
	char *history;

	switch ( len )
	{
	case 3:
		// check 方向键
		//
		// m_deal_arrow_keys()
		if ( str[0] == 0x1B && str[1] == 0x4F && str[2] == 0x41 )
		{
			//em_printf("press up.\n");
			history = emHistory.previous(&emHistory);
			if ( history != NULL )
			{
				emConsoleString.setString(&emConsoleString, history);
			}
			
			return 1;
		}
		else if ( str[0] == 0x1B && str[1] == 0x4F && str[2] == 0x42 )
		{
			//em_printf("press down.\n");
			history = emHistory.next(&emHistory);
			emConsoleString.setString(&emConsoleString, history);

			return 1;
		}
		else if ( str[0] == 0x1B && str[1] == 0x4F && str[2] == 0x43 )
		{
			if ( emConsoleString.cursor < emConsoleString.len )
			{
				em_printf("\033[C");
				emConsoleString.cursor ++;
			}

			//em_printf("press right.\n");
			return 1;
		}
		else if ( str[0] == 0x1B && str[1] == 0x4F && str[2] == 0x44 )
		{
			//em_printf("press left.\n");
			if ( emConsoleString.cursor > 0 )
			{
				emConsoleString.cursor --;
				em_printf("\033[D");
			}

			//em_printf("\033[D");
			//em_printf("\033[D");
			return 1;
		}
		break;
	case 2:
		if ( strcmp(str, "\r\n") == 0 )
		{
			//em_printf("press Enter.\n");
			em_printf("\nGet:: %s.\n", emConsoleString.getString(&emConsoleString));
			emHistory.append(&emHistory, emConsoleString.str);
			emConsoleString.reset(&emConsoleString);
			return 1;
		}
		break;

	case 1:
		if ( str[0] == KEY_BACKSPACE )
		{
			if ( emConsoleString.cursor > 0 )
			{
				em_printf("%c", KEY_BACKSPACE);
				emConsoleString.remove(&emConsoleString);


			}
			return 1;
		}
		break;
	default:

		break;

	}
	return 0;
}

void shell_line_handle(char* str, uint16_t len)
{
	static char buf[20];
	static uint16_t cursor = 0;
	char * his;

	if ( m_deal_special_button(str, len) == 0 )
	{
		for ( int i = 0; i < len; i ++ )
		{
			em_printf("%c", str[i]);
			emConsoleString.insert(&emConsoleString, str[i]);
		}
	}

}
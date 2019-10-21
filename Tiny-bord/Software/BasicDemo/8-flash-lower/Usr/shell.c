#include "shell.h"

// cursor
//
// line
//
//
/////////////////////////////////////////////////////////////////
static void hInitialise(historyStack_t * this)
{
	this->root = 0;
	this->top  = 0;
	this->cur  = 0;
	this->is_Empty= 0;
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
		if ( strcmp(this->cmd_history[this->top], str) != 0 )
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

historyStack_t emHistory = {
	{"","","","","","","","","",""},
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

void shell_line_handle(char* str, uint16_t len)
{
    static char buf[20];
    static uint16_t cursor = 0;
	char * his;
    // init cursor
    //
    uint16_t pos = 0;
    //em_printf("SHELL: str -> %s.\n", str);
    //
    
    if ( len == 3 )
    {	 // UP
    	if ( str[0] == 0x1B && str[1] == 0x4F && str[2] == 0x41 )
    	{
    		//em_printf("KEY_UP.\n");
    		his = emHistory.previous(&emHistory);
			buf[cursor] = '\0';
			memcpy(buf, his, strlen(his));
			for ( int i = 0; i < cursor+1; i ++ )	em_printf("%c", KEY_BACKSPACE);
			em_printf("%s", his);
			cursor = strlen(his);
    		return;
    	}// DOWN
    	else if ( str[0] == 0x1B && str[1] == 0x4F && str[2] == 0x42 )
    	{
    		his = emHistory.next(&emHistory);
			if ( his == NULL )
			{
				em_printf("%c", KEY_BACKSPACE);
				return;
			}
			else
			{
    			memcpy(buf, his, strlen(his));
    			for ( int i = 0; i < cursor+1; i ++ )	em_printf("%c", KEY_BACKSPACE);
    			em_printf("%s", his);
    			cursor = strlen(his);				
			}
    		return;
    	}// RIGHT
    	else if ( str[0] == 0x1B && str[1] == 0x4F && str[2] == 0x43 )
    	{
    		em_printf("KEY_RIGHT.\n");
    		return;
    	}// LEFT
    	else if ( str[0] == 0x1B && str[1] == 0x4F && str[2] == 0x44 )
    	{
    		em_printf("KEY_LEFT.\n");
    		return;
    	}
    }


    while (len--) {

        //em_printf("SHELL>>> 0x%X.\n", str[pos ++]);
        //

        switch (str[pos]) {
        case KEY_BACKSPACE:
            if (cursor != 0) {
                cursor--;
            } else {
                em_printf(" ");
            }
            continue;
            break;

        default:
            buf[cursor++] = str[pos++];
            if (str[pos - 1] == 0xA) {
                buf[cursor-2] = '\0';
                cursor = 0;
				emHistory.append(&emHistory, buf);
                em_printf("\nSHELL>>> %s\r\n", buf);
                em_printf("root>>> ");
            }
        }

        //em_printf("SHELL>>> BUF[%d]:0x%X.\n", cursor-1, buf[cursor-1]);

        /*
		switch ( str[pos ++] )
		{
			case expression:
		}
		*/
    }
}
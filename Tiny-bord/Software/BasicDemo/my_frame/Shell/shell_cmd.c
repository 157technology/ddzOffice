#include "shell_cmd.h"
#include "shell_config.h"

#include "shell.h"

#include <stdio.h>
#include <string.h>

char shell_cmd[SHELL_CMDS_AMOUNT][SHELL_CMDS_MAX_LEN];


static shell_cmd_t * shell_cmd_find(char * str);

void shell_cmd_handle(char *str)
{
	SHELL_SKIP_SPACE(str);
	int cnt = 0;
	while ( sscanf(str, "%s", shell_cmd[cnt]) != EOF )
	{
		str += strlen(shell_cmd[cnt++]);
		SHELL_SKIP_SPACE(str);
	}
	shell_cmd_t * pcmd = shell_cmd_find(shell_cmd[0]);
	if ( pcmd == NULL )
	{
		em_printf("\nNo such instruction : %s.\n", shell_cmd[0]);
	}
	else
	{
		em_printf("\nRun %s \n", shell_cmd[0]);
		pcmd->run(0, NULL);
	}
}


static shell_cmd_t * shell_cmd_find(char * str)
{
	for ( shell_cmd_t ** ppcmd = (shell_cmd_t **)&ShellCmdTab$$Base; ppcmd != (shell_cmd_t **)&ShellCmdTab$$Limit; ppcmd ++ )
	{
		shell_cmd_t * pcmd = *ppcmd;

		if ( strcmp(pcmd->cmd, str) == 0 )
		{
			// this is the cmd
			return pcmd;
		}
	}

	return NULL;
}
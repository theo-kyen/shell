#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "cmd.h"

struct simple_cmd
{
	int num_avail_args;
	int num_args;
	char **args;
};

struct cmd
{
	char *name;
	int num_avail_simple_cmds;
	int num_simple_cmds;
	simple_cmd_t **simple_cmds;
	char *out_file;
	char *in_file;
	char *err_file;
	int bg;
};

// reverses the argument list in the given cmd
void reverse_args(simple_cmd_t *cmd);
// allocates more space for the argument list in the given cmd
// returns 0 on success or 1 on failure
int alloc_arg_space(simple_cmd_t *cmd);

simple_cmd_t *simple_cmd_init()
{
	simple_cmd_t *cmd = (simple_cmd_t *)malloc(sizeof(simple_cmd_t));
	cmd->num_avail_args = 2;
	cmd->num_args = 0;
	if (alloc_arg_space(cmd) > 0)
		return NULL;

	return cmd;
}

int set_simple_cmd(simple_cmd_t *cmd, char *name)
{
	cmd->args[0] = name;
	return 0;
}

int insert_arg(simple_cmd_t *cmd, char *arg)
{
	if (cmd == NULL || arg == NULL)
		return 1;

	int num_args = cmd->num_args;
	// init argument list if necessary
	if (num_args == 0 && cmd->args == NULL)
	{
		if (alloc_arg_space(cmd) > 0)
			return 1;
	}

	// copy the argument
	char *dst = (char *)malloc((strlen(arg) + 1) * sizeof(char));
	if (dst == NULL)
		return 1;
	strcpy(dst, arg);
	cmd->args[num_args + 1] = dst;
	cmd->num_args++;

	// double argument list capacity if necessary
	if (cmd->num_args == cmd->num_avail_args)
	{
		cmd->num_avail_args *= 2;
		size_t size = cmd->num_avail_args + 1;
		char **temp = realloc(cmd->args, size * sizeof(char *));
		if (temp == NULL)
			return 1;
		cmd->args = temp;
		cmd->args[cmd->num_avail_args] = NULL;
	}

	return 0;
}

// frees a simple_cmd
void simple_cmd_free(simple_cmd_t **_cmd)
{
	simple_cmd_t *cmd = *_cmd;
	if (cmd == NULL)
		return;

	if (cmd->args != NULL)
	{
		for (int i = 0; i < cmd->num_args + 1; i++)
		{
			free(cmd->args[i]);
			cmd->args[i] = 0;
		}
		free(cmd->args);
		cmd->args = 0;
	}

	cmd->num_avail_args = 0;
	cmd->num_args = 0;

	free(cmd);
	cmd = 0;
	*_cmd = 0;
}

// inits a cmd
cmd_t *cmd_init()
{
	cmd_t *cmd = (cmd_t *)malloc(sizeof(cmd_t));

	cmd->name = NULL;
	cmd->num_avail_simple_cmds = 2;
	cmd->num_simple_cmds = 0;
	cmd->simple_cmds = NULL;
	cmd->out_file = NULL;
	cmd->in_file = NULL;
	cmd->err_file = NULL;
	cmd->bg = 0;

	return cmd;
}

int set_cmd(cmd_t *cmd, char *name)
{
	if (cmd->name != 0)
	{
		printf("Error: cannot change command after already being set\n");
		return 1;
	}

	cmd->name = name;
	return 0;
}

// inserts the given simple_cmd into the given cmd, grows simple_cmds if necessary
int insert_simple_cmd(cmd_t *cmd, simple_cmd_t *simple_cmd)
{
	if (cmd == NULL || simple_cmd == NULL)
	{
		return 1;
	}

	int num_simple_cmds = cmd->num_simple_cmds;
	if (num_simple_cmds == 0)
	{
		cmd->simple_cmds = (simple_cmd_t **)malloc(2 * sizeof(simple_cmd_t *));
	}

	cmd->simple_cmds[num_simple_cmds] = simple_cmd;
	cmd->num_simple_cmds++;

	if (cmd->num_simple_cmds == cmd->num_avail_simple_cmds)
	{
		cmd->num_avail_simple_cmds *= 2;
		simple_cmd_t **temp = realloc(cmd->simple_cmds, cmd->num_avail_simple_cmds * sizeof(simple_cmd_t *));
		if (temp == NULL)
			return 1;
		cmd->simple_cmds = temp;
	}
}

// frees a cmd
void cmd_free(cmd_t **_cmd)
{
	cmd_t *cmd = *_cmd;

	if (cmd == NULL)
		return;

	if (cmd->simple_cmds != NULL)
	{
		for (int i = 0; i < cmd->num_simple_cmds; i++)
		{
			simple_cmd_free(&cmd->simple_cmds[i]);
			cmd->simple_cmds[i] = 0;
		}
		free(cmd->simple_cmds);
		cmd->simple_cmds = 0;
	}

	cmd->name = NULL;
	cmd->num_avail_simple_cmds = 0;
	cmd->num_simple_cmds = 0;
	cmd->out_file = 0;
	cmd->in_file = 0;
	cmd->err_file = 0;
	cmd->bg = 0;

	free(cmd);
	*_cmd = 0;
}

int simple_execute(simple_cmd_t *cmd)
{
	if (strcmp(cmd->args[0], "exit") == 0)
	{
		exit(EXIT_SUCCESS);
	}

	pid_t pid = fork();
	if (pid < 0)
	{
		printf("error\n");
		return 1;
	}

	// child process
	if (pid == 0)
	{
		// printf("child\n");
		if (cmd->args[0] == NULL)
		{
			return 1;
		}

		reverse_args(cmd);
		execvp(cmd->args[0], cmd->args);
	}

	// parent process
	waitpid(pid, NULL, 0);
	return 0;
}

int execute(cmd_t *cmd)
{
	return 0;
}

void reverse_args(simple_cmd_t *cmd)
{
	char **left = cmd->args + 1;
	char **right = cmd->args + cmd->num_args;
	char *temp;

	while (left < right)
	{
		temp = *left;
		*left = *right;
		*right = temp;
		left++;
		right--;
	}
}

int alloc_arg_space(simple_cmd_t *cmd)
{
	cmd->args = (char **)malloc(4 * sizeof(char *));
	if (cmd->args == NULL)
		return 1;
	cmd->args[cmd->num_avail_args] = NULL;
	return 0;
}
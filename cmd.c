#include <stdlib.h>
#include <string.h>
#include "cmd.h"

cmd_t *curr_cmd;
simple_cmd_t *curr_simple_cmd;

// inits a simple_cmd
simple_cmd_t *simple_cmd_init() {
	simple_cmd_t *cmd = (simple_cmd_t *)malloc(sizeof(simple_cmd_t));
	cmd->num_avail_args = 2;
	cmd->num_args = 0;
	cmd->args = NULL;
	return cmd;
}

// inserts the given arg into the given simple_cmd, grows args if necessary
int insert_arg(simple_cmd_t *cmd, char *arg) {
	if (cmd == NULL || arg == NULL) return 1;

	int num_args = cmd->num_args;
	if (num_args == 0) {
		cmd->args = (char **)malloc(3 * sizeof(char *));
		if (cmd->args == NULL) return 1;
	}

	char *dst = (char *)malloc((strlen(arg) + 1) * sizeof(char));
	if (dst == NULL) return 1;
	strcpy(dst, arg);
	cmd->args[num_args] = dst;
	cmd->num_args++;
	
	if (cmd->num_args == cmd->num_avail_args) {
		cmd->num_avail_args *= 2;
		size_t size = cmd->num_avail_args + 1;
		char **temp = realloc(cmd->args, size * sizeof(char *));
		if (temp == NULL) return 1;
		cmd->args = temp;
	}

	return 0;	
}

// frees a simple_cmd
void simple_cmd_free(simple_cmd_t *cmd) {
	if (cmd->args != NULL) {
		for (int i = 0; i < cmd->num_args; i++) {
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
}

// inits a cmd
cmd_t *cmd_init() {
	cmd_t *cmd = (cmd_t *)malloc(sizeof(cmd_t));
	
	cmd->num_avail_simple_cmds = 2;
	cmd->num_simple_cmds = 0;
	cmd->simple_cmds = NULL;
	cmd->out_file = NULL;
	cmd->in_file = NULL;
	cmd->err_file = NULL;
	cmd->bg = 0;

	return cmd;
}

// inserts the given simple_cmd into the given cmd, grows simple_cmds if necessary
int insert_simple_cmd(cmd_t *cmd, simple_cmd_t *simple_cmd) {
	if (cmd == NULL || simple_cmd == NULL) {
		return 1;
	}

	int num_simple_cmds = cmd->num_simple_cmds;
	if (num_simple_cmds == 0) {
		cmd->simple_cmds = (simple_cmd_t **)malloc(2 * sizeof(simple_cmd_t *));
	}

	cmd->simple_cmds[num_simple_cmds] = simple_cmd;
	cmd->num_simple_cmds++;

	if (cmd->num_simple_cmds == cmd->num_avail_simple_cmds) {
		cmd->num_avail_simple_cmds *= 2;
		simple_cmd_t **temp = realloc(cmd->simple_cmds, cmd->num_avail_simple_cmds * sizeof(simple_cmd_t *));
		if (temp == NULL) return 1;
		cmd->simple_cmds = temp;
	}
}

// frees a cmd
void cmd_free(cmd_t *cmd) {
	if (cmd->simple_cmds != NULL) {
		for (int i = 0; i < cmd->num_simple_cmds; i++) {
			simple_cmd_free(cmd->simple_cmds[i]);
			cmd->simple_cmds[i] = 0;
		}
		free(cmd->simple_cmds);
		cmd->simple_cmds = 0;
	}

	cmd->num_avail_simple_cmds = 0;
    cmd->num_simple_cmds = 0;
    cmd->out_file = 0;
    cmd->in_file = 0;
    cmd->err_file = 0;
    cmd->bg = 0;

	free(cmd);
	cmd = 0;
}

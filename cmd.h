// Defined in Ch.5 of Introduction to Systems Programming: a Hands-on Approach
#pragma once

// describes a simple command and its arguments
typedef struct simple_cmd {
	int num_avail_args;
	int num_args;
	char **args;
} simple_cmd_t;

// inits a simple_cmd
simple_cmd_t *simple_cmd_init();
// inserts the given arg into the given simple_cmd, grows args if necessary
int insert_arg(simple_cmd_t *cmd, char *arg);
// frees a simple_cmd
void simple_cmd_free(simple_cmd_t *cmd);

// describes a complete command with multiple pipes and IO redirection (if any)
typedef struct cmd {
	int num_avail_simple_cmds;
	int num_simple_cmds;
	simple_cmd_t **simple_cmds;
	char *out_file;
	char *in_file;
	char *err_file;
	int bg;
} cmd_t;

// inits a cmd
cmd_t *cmd_init();
// inserts the given simple_cmd into the given cmd, grows simple_cmds if necessary
int insert_simple_cmd(cmd_t *cmd, simple_cmd_t *simple_cmd);
// frees a cmd
void cmd_free(cmd_t *cmd);

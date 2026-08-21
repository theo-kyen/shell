// Defined in Ch.5 of Introduction to Systems Programming: a Hands-on Approach
#pragma once

// describes a simple command and its arguments
typedef struct simple_cmd simple_cmd_t;

// inits a simple_cmd
simple_cmd_t *simple_cmd_init();
// sets the command of the given simple_cmd to name
int set_simple_cmd(simple_cmd_t *cmd, char *name);
// inserts the given argument into the given simple_cmd, grows arg list if necessary
int insert_arg(simple_cmd_t *cmd, char *arg);
// frees a simple_cmd
void simple_cmd_free(simple_cmd_t **_cmd);

// describes a complete command with multiple pipes and IO redirection (if any)
typedef struct cmd cmd_t;

// inits a cmd
cmd_t *cmd_init();
// sets the command of the cmd given to name
int set_cmd(cmd_t *cmd, char *name);
// inserts the given simple_cmd into the given cmd, grows simple_cmd list if necessary
int insert_simple_cmd(cmd_t *cmd, simple_cmd_t *simple_cmd);
// frees a cmd
void cmd_free(cmd_t **_cmd);

// executes cmds
int simple_execute(simple_cmd_t *cmd);
int execute(cmd_t *cmd);

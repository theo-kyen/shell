%{
int yylex(void);
void yyerror(const char *s);

#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#include "cmd.h"

cmd_t *cmd;
simple_cmd_t **simple_cmds;
int cmd_idx = 0;
int complex = 0;

struct passwd *user;

void init(void);

// insert the given argument into the simple_cmd
void y_insert_arg(int idx, char *arg);
// set the simple_cmd command
void y_set_simple_cmd(int idx, char *name);

// insert the given simple_cmd into the cmd
void y_insert_simple_cmd(void);
void y_set_cmd(char *name);

int y_execute(void);
void y_cmd_free(void);
%}

%union {
    char *str;
}
%start cmd_list
%token NEWLINE
%token GREATER_THAN
%token LESS_THAN
%token GREATGREAT
%token GREATAMP
%token PIPE
%token AMP
%token <str> WORD

%%
cmd_list:   cmd_list cmd_line {
                y_execute();
                y_cmd_free();
                printf("%s$ ", user->pw_name);
            }
            |
            ;

args        :       
            | args WORD {
                    y_insert_arg(cmd_idx, $2);
                }
            ;

cmd_w_args  : WORD args {
                    y_set_simple_cmd(cmd_idx, $1);
                }
            ;

pipe_list   : pipe_list PIPE cmd_w_args {
                complex = 1;
            }
            | cmd_w_args
            ;

io_mod:    GREATGREAT WORD
            | GREATER_THAN WORD
            | GREATAMP WORD
            | LESS_THAN WORD
            ;

io_mod_list : io_mod_list io_mod
            |
            ;

bg          : AMP
            |
            ;

cmd_line:   pipe_list io_mod_list bg NEWLINE
            | NEWLINE
            | error NEWLINE{yyerrok;}

%%
int main(void) {
    init();

    // TODO: check if there are arguments to run, if so, run them and exit
    printf("%s$ ", user->pw_name);
    yyparse();
}

void init(void) {
    simple_cmds = (simple_cmd_t **)malloc(2 * sizeof(simple_cmd_t *));

    // clear the screen
    int pid = fork();
    if (pid == 0) {
        char *args[] = {"clear", NULL};
        execvp(args[0], args);
    }

    waitpid(pid, NULL, 0);

    user = getpwuid(getuid());
}

void y_insert_arg(int idx, char *arg) {
    if (simple_cmds == NULL) {
        simple_cmds = (simple_cmd_t **)malloc(2 * sizeof(simple_cmd_t *));
        simple_cmds[0] = simple_cmd_init();
    }

    if (simple_cmds[idx] == NULL)
        simple_cmds[idx] = simple_cmd_init();

    insert_arg(simple_cmds[idx], arg);
 }
 
void y_set_simple_cmd(int idx, char *name) {
    if (simple_cmds == NULL) {
        simple_cmds = (simple_cmd_t **)malloc(2 * sizeof(simple_cmd_t *));
        simple_cmds[0] = simple_cmd_init();
    }
        
    if (simple_cmds[idx] == NULL)
        simple_cmds[idx] = simple_cmd_init();
        
    set_simple_cmd(simple_cmds[idx], name);
    cmd_idx++;
}

void y_set_cmd(char *name) {
    if (cmd == NULL) {
        cmd = cmd_init();
    }

    if (set_cmd(cmd, name) > 0) {
        y_cmd_free();
    }
}

int y_execute(void) {
    if (!complex) {
        simple_execute(simple_cmds[0]);
    }
    else
        execute(cmd);
}

void y_cmd_free(void) {
    for (int i = 0; i < cmd_idx; i++) {
        simple_cmd_free(&simple_cmds[i]);
        simple_cmds[i] = 0;
    }
    free(simple_cmds);
    simple_cmds = 0;

    cmd_free(&cmd);
    cmd = 0;

    complex = 0;
    cmd_idx = 0;
}

void yyerror(const char *s) {
    fprintf(stderr, "Error: %s\n", s);
}

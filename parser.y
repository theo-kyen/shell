%{
int yylex(void);
void yyerror(const char *s);

#include <stdio.h>
#include <stdlib.h>
#include "cmd.h"

cmd_t *cmd;
simple_cmd_t *simple_cmd;
int num_simple_cmds = 0;

void y_insert_arg(char *arg);
void y_set_simple_cmd(char *name);
void y_set_cmd(char *name);
int y_execute();
void y_cmd_free();
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
                printf("$ ");
            }
            |
            ;

args        :       
            | WORD args {
                    y_insert_arg($1);
                }
            ;

cmd_w_args  : WORD args {
                    y_set_simple_cmd($1);
                }
            ;

pipe_list   : pipe_list PIPE cmd_w_args
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
    // TODO: check if there are arguments to run, if so, run them and exit
    printf("$ ");
    yyparse();
}

void y_insert_arg(char *arg) {
    if (simple_cmd == NULL)
        simple_cmd = simple_cmd_init();

    /* simple_cmd_t *sim = simple_cmds[num_simple_cmds]; */
    insert_arg(simple_cmd, arg);
 }
 
void y_set_simple_cmd(char *name) {
    /* simple_cmd_t *sim = simple_cmds[num_simple_cmds]; */
    if (simple_cmd == NULL)
        simple_cmd = simple_cmd_init();
        
    set_simple_cmd(simple_cmd, name);
    num_simple_cmds++;
}

void y_set_cmd(char *name) {
    if (cmd == NULL) {
        cmd = cmd_init();
    }

    if (set_cmd(cmd, name) > 0) {
        y_cmd_free();
    }
}

int y_execute() {
    if (simple_cmd == NULL)
        simple_cmd = simple_cmd_init();

    simple_execute(simple_cmd);
}

void y_cmd_free() {
    simple_cmd_free(&simple_cmd);
    simple_cmd = 0;
}

void yyerror(const char *s) {
    fprintf(stderr, "Error: %s\n", s);
}

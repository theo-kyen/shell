%{
int yylex(void);
void yyerror(char const *s);
#include <stdio.h>
#include <stdlib.h>
%}
%start cmd_list
%token NEWLINE GREATER_THAN LESS_THAN GREATGREAT GREATAMP PIPE AMP WORD NONE
%%

cmd_list:	cmd_list cmd_line
			| cmd_line;
args:		args WORD
			| 
			;
cmd:		WORD args
			;
pipe:		pipe PIPE cmd
			| cmd
			;
io:			GREATER_THAN WORD
			| LESS_THAN WORD
			| GREATGREAT WORD
			| GREATAMP WORD
			;
io_mods:	io_mods io
			| io
			;
bg:			AMP
			|
			;
cmd_line:	pipe io_mods bg NEWLINE
			| NEWLINE
			| error NEWLINE {yyerrok;}
			;
%%

int main(void) {
	return yyparse();
}

void yyerror(const char *s) {
    fprintf(stderr, "Error: %s\n", s);
}


%{
#include <string.h>
#include "y.tab.h"
%}

%%
\n		{ return NEWLINE; }
">"		{ return GREATER_THAN; }
"<"		{ return LESS_THAN; }
">>"	{ return GREATGREAT; }
">&"	{ return GREATAMP; }
"|"		{ return PIPE; }
"&"		{ return AMP; }
[^ \t\n][^ \t\n]* {
//	yylval->string_val = strdup(yytext); 
	return WORD; 
}

.   	{ return NONE; }
%%

int yywrap(void) {
	return 1;
}

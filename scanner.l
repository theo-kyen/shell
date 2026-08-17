%{
#include <string.h>
#include "y.tab.h"
%}

%%
\n      { return NEWLINE; }
">>"    { return GREATGREAT; }
">&"    { return GREATAMP; }
">"     { return GREATER_THAN; }
"<"     { return LESS_THAN; }
"|"     { return PIPE; }
"&"     { return AMP; }

[^ \t\n][^ \t\n]* {
    yylval.str = strdup(yytext); 
    return WORD; 
}

.   {}
%%

int yywrap(void) {
	return 1;
}

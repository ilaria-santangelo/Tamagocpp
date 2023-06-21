%{
#include "parser.tab.h"
#include <string>
void yyerror(const char *s);
extern int yyparse();
extern FILE *yyin;
extern int yylex();
%}

%option noyywrap

%%


"adopt" { return ADOPT; }
"feed"  { return FEED; }
"play" { return PLAY; }
"sleep" { return SLEEP; }
"status" { return STATUS; }
"dog"   { yylval.str = strdup("dog"); return IDENTIFIER; }
"cat"   { yylval.str = strdup("cat"); return IDENTIFIER; }
[a-zA-Z][a-zA-Z0-9]* { yylval.str = strdup(yytext); return IDENTIFIER; }
[ \t\n\r]+ { /* ignore whitespace */ }
";" { return ';'; } 
. { yyerror("Invalid character"); }

%%

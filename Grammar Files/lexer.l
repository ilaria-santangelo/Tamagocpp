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
"save" { return SAVE; }
"load" { return LOAD; }

"dog"   { yylval.str = strdup("dog"); return IDENTIFIER; }
"cat"   { yylval.str = strdup("cat"); return IDENTIFIER; }
"adopt" { return ADOPT; }
"feed"  { return FEED; }
"play" { return PLAY; }
"sleep" { return SLEEP; }
"gift" { return GIFT; }
"use"  { return USE; }
"status" { return STATUS; }
"teach" { return TEACH; }
"perform" { return PERFORM; }
"bury" { return BURY; }
"number" { return NUMBER; }
[0-9]+ { yylval.num = atoi(yytext); return INTEGER; }
"=" { return '='; }
[a-zA-Z][a-zA-Z0-9]* { yylval.str = strdup(yytext); return IDENTIFIER; }
[ \t\n\r]+ { /* ignore whitespace */ }
";" { return ';'; } 
. { yyerror("Invalid character"); }

%%

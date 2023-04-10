/*
 * Reentrant lexer for measurement units expressions
 */
%option noyywrap nodefault reentrant bison-bridge

%{
    
#include "read_units.h"
#include "read_units.tab.h"
    
%}

/* Definition (Substitution) Section */


%%
 /* single character ops */
"+" |
"-" |
"*" |
"/" |
"^" |
"(" |
")"   { return yytext[0]; }
[0-9]+	     { yylval->d = atoi(yytext); return T_number; }
[a-zA-Z]+	 { yylval->s = strdup(yytext); return T_symbol; }
\n      { return EOL; }
[ \t]   { /* ignore white space */ }
.	{ printf("Mystery character %c\n", *yytext); }
%%


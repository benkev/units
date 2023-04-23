/*
 * Reentrant lexer for measurement units expressions
 */
%option noyywrap nodefault reentrant bison-bridge

%{
    
#include <string.h>
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


/* void parse_expr(const std::string& inp) */
void parse_expr(const char inp[])
{
    yyscan_t scanner;
    
    yylex_init(&scanner);
    struct yyguts_t * yyg = (struct yyguts_t *)scanner;

    /*  */
    // yy_delete_buffer(YY_CURRENT_BUFFER,scanner);
    yypop_buffer_state(scanner);

    /*Copy string into new buffer and Switch buffers*/
    yy_scan_string(inp.c_str(), scanner);

    yyparse(scanner);
    
    yylex_destroy(scanner);
    /* return nasi; */
}

int main(int argc, char **argv) {
    
    /* std::string inp = argv[1]; */
    char *inp = argv[1];

    parseExpression(inp);
    
    /* std::cout << inp.substr(0, inp.size()-1) << " == " << nasi */
    /*           << std::endl; */

    printf("\n");

    return 0;
}

 

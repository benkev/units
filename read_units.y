/* 
 * Parse an expression of measurement units
*/

%{
    
#include <stdio.h>
#include <math.h>

void yyerror(char const *s);
int yylex(void);
 
/* int yylex(yyscan_t scanner); */

%}

/*
 * Parse stack element
 */
%union {
  char  *s;
  int    d;
}

/* Declare tokens (terminal symbols) */
%token <d> NUM
%token <s> MEASURE
%token <d> EOL

/* Declare type for the expression (nonterminal symbol) */
/* %type <s> exp */
%type <d> numex
%type <d> explist

/* Declare precedence and associativity */
/* Operators are declared in increasing order of precedence */
%left '+' '-'
%left '*' '/'
%left NEG     /* negation - unary minus */
%left POS     /* unary plus             */
%right '^'    /* exponentiation         */

/* Grammar */
%%

/* exp:    MEASURE              {          } */
/*         | exp '*' exp        {     } */
/*         | exp '/' exp        {     } */
/*         | exp '^' numex      {  } */
/*         | '(' exp ')'        {          } */
/* ; */

explist:   /* empty */
        | explist numex EOL   { printf("= %d\n> ", $2); }
        | explist EOL         { printf("> "); } /* blank line or a comment */

numex:  NUM                      { $$ = $1;         }
        | numex '+' numex        { $$ = $1 + $3;    }
        | numex '-' numex        { $$ = $1 - $3;    }
        | '-' numex  %prec NEG   { $$ = -$2;        }
        | '+' numex  %prec POS   { $$ = $2;        }
        | numex '*' numex        { $$ = $1 * $3;    }
        | numex '/' numex        { $$ = $1 / $3;    }
        | numex '^' numex        { $$ = (int) pow($1, $3); }
        | '(' numex ')'          { $$ = $2;         }
;
%%


int main(int argc, char **argv) {

    yyparse();
    
    return 0;
}

void yyerror(char const *s) {
    fprintf(stderr, "error: %s\n", s);
}

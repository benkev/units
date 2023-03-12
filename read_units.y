/* 
 * Parse an expression of measurement units
*/

%{
    
#include <stdio.h>
#include <math.h>

void yyerror(char const *s);
int yylex(void);
 
/* int yylex(yyscan_t scanner); */

/*
 * The positions of the measurement unit powers in dim[] and 
 * multiples or subdivisions in mul[]  are as follows:
 *
 * 0: length (meter,    m)
 * 1: mass	 (kilogram, kg)
 * 2: time   (second,	s)
 * 3: electric current	(Ampere, A)
 * 4: thermodynamic temperature	(Kelvin, K)
 * 5: amount of substance (mole, mol)
 * 6: luminous intensity (candela, cd)
 * 7: plane angle (radian, rad)
 * 8: plane angle (degree, deg)
 * 9: solid angle (steradian, sr)
 * 
 * Array dim stores integer powers of the units in the measure expression.
 * Array mul stores integer powers of the prefix multipliers of units 
 * in the measure expression.
 *
 *
 */
 
int dim[32];  /* powers of the units */
int mul[32];  /* powers of the prefix multipliers, like milli, kilo etc. */


%}

/*
 * Parse stack element
 */
%union {
  char  *s;
  int    d;
}

/* Declare tokens (terminal symbols) */
%token <d> NUMBER
%token <s> MEASURE
%token EOL

/* Declare type for the expression (nonterminal symbol) */
/* %type <s> exp */
%type <d> numex

/* Declare precedence and associativity */
/* Operators are declared in increasing order of precedence */
%left '+' '-'
%left '*' '/'
%left NEG     /* negation - unary minus */
%left POS     /* unary plus             */
%right '^'    /* exponentiation         */

/* Grammar */
%%

explist:   /* empty */
        | explist numex EOL   { printf("= %d\n> ", $2); }
        | explist symex EOL   { printf("= %d\n> ", $2); }
        | explist EOL         { printf("> "); } /* blank line or a comment */

symex:    MEASURE              {          }
        | symex '*' symex        {     }
        | symex '/' symex        {     }
        | symex '^' numex      {  }
        | '(' symex ')'        {          }
;

numex:  NUMBER                   { $$ = $1;         }
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

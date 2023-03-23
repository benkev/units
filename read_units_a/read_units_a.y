/* 
 * Parse an expression of measurement units
 */

%{
    
#include <stdio.h>
#include <math.h>
#include "read_units_a.h"
    
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
 
enum measure_index {i_length = 0, i_mass, i_time, i_current, i_temp, i_lumi,
             i_mole, i_freq, i_ang_rad, i_ang_deg, i_solid_ang, i_Jansky};

int mea[32];  /* 1: a measure present, 0 - absent */
int dim[32];  /* powers of the units */
int mul[32];  /* powers of the prefix multipliers, like milli, kilo etc. */
enum measure_index i_measure;

%}

/*
 * Parse stack element
 */
%union {
    ast_node *a;
    char  *s;
    int    d;
}

/* Declare tokens (terminal symbols) */
%token <d> EOL
%token <d> T_number
%token <s> T_SI_prefix
%token <s> T_symbol


/* Declare type for the expression (nonterminal symbol) */
/* %type <s> exp */
%type <d> numex
%type <d> measure 
%type <a> symex

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
        | explist numex EOL   {
          printf("= %d\n> ", $2);
        }
        | explist symex EOL   { ast_node *a = $2;
     printf("nodetype = %c, l = %p, r = %p\n", a->nodetype, a->l, a->r);
     //expr_list *el = reduce($2);
     //printf("Reduced.\n> ");
            
            printf("\n> ");
        }
        | explist EOL         { printf("\n> "); } /* blank line */
;

symex:  measure              { $$ = newmeas($1); }
        | symex '*' symex    { $$ = newast('*', $1, $3); }
        | symex '/' symex    { $$ = newast('/', $1, $3);  }
        | symex '^' numex    { printf("pwr='%d'\n",$3); ast_node *ipow = newnum($3);
                               $$ = newast('^', $1, ipow); }
        | '(' symex ')'      { $$ = $2; }
;

measure: T_symbol    { printf("sym='%s'\n",$1); $$ = getmeas($1); }
;

numex:  T_number                 { printf("num='%d'\n",$1); $$ = $1;         }
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
    
/* #ifdef YYDEBUG */
/*     yydebug = 1; */
/* #endif */
    
    yyparse();
    
    return 0;
}


/* 
 * Parse an expression of measurement units
 */
%{
    
#include <stdio.h>
#include <math.h>
#include "read_units.h"
    
int yylex(void);
 
/*
 * The positions of the measurement unit powers in dim[] and 
 * multiples or subdivisions in mul[]  are as follows:
 *
 * 0: length (meter,    m)
 * 1: mass	 (kilogram, kg)
 * 2: time   (second,	s)
 * 3: time   (min, hr, day, mon, yr)
 * 4: electric current	(Ampere, A)
 * 5: thermodynamic temperature	(Kelvin, K)
 * 6: luminous intensity (candela, cd)
 * 7: amount of substance (mole, mol)
 * 8: frequency (Hertz, Hz)
 * 9: plane angle (radian, rad)
 * 10: plane angle (degree, deg)
 * 11: solid angle (steradian, sr)
 * 12: spectral flux density, or spectral irradiance (Jansky, Jy)
 * 
 * Array dim stores integer powers of the units in the measure expression.
 * Array mul stores integer powers of the prefix multipliers of units 
 * in the measure expression.
 *
 *
 */
 
/* enum measure_index {i_length = 0, i_mass, i_time, i_tday, i_current, */
/*                     i_temp, i_lumi, i_mole, i_freq, i_ang_rad, i_ang_deg, */
/*                     i_solid_ang, i_Jansky}; */

%}

%parse-param { expr_list *explst }

/*
 * Parse stack element
 */
%union {
    ast_node *a;
    char  *s;
    int    d;
}

/* Declare tokens (terminal symbols) */
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

exprsn:  numex YYEOF {
                      yyerror(explst, "no measurement units, just number: %d", $1);
                      YYERROR;
                     }
        | symex YYEOF   { ast_node *a = $1;
                          print_tree($1);
                          explst = reduce($1, 0); /* List ptr: yylex(expr_list*) */
                          /* expr_list *el = reduce($1, 0); */
                          printf("Reduced to list:\n ");
                          expr_list *el1 = explst;
                          print_list(explst);
                          if (explst == el1) printf("el == el1\n");
                          if (explst != el1) printf("el != el1\n");
                          printf("el=%p, el1=%p\n", explst, el1);
                          printf("\nDone. ");
        }
        | YYEOF         { yyerror(explst, "empty string."); YYERROR; }
;

symex:  measure              { $$ = newmeas($1); }
        | symex '*' symex    { $$ = newast('*', $1, $3); }
        | symex '/' symex    { $$ = newast('/', $1, $3);  }
        | symex '^' numex    { ast_node *ipow = newnum($3);
                               $$ = newast('^', $1, ipow); }
        | '(' symex ')'      { $$ = $2; }
;

measure: T_symbol    { $$ = getmeas($1);
                       if ($$ == -1) {
                           yyerror(explst, "no such measurement unit: '%s'", $1);
                           YYERROR;
                       }
                     }
;

numex:  T_number                 { $$ = $1;         }
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


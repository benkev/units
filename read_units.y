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
  char  *s;
  int    d;
}

/* Declare tokens (terminal symbols) */
%token <d> EOL
%token <d> T_number
%token <s> T_SI_prefix
%token <s> T_length T_mass T_time T_current T_temp T_lumi T_mole
%token <s> T_freq T_ang_rad T_ang_deg T_solid_ang T_Jansky

/* Declare type for the expression (nonterminal symbol) */
/* %type <s> exp */
%type <d> numex
%type <s> measure symex

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
        | explist symex EOL   { printf("= %s\n> ", $2);
                                for (int i=0; i<12; i++) printf("%d ", dim[i]);
                                printf("\n> ");
                                for (int i=0; i<32; i++)
                                    mea[i] = dim[i] = mul[i] = 0;
                              }
        | explist EOL         { for (int i=0; i<12; i++)
                                  mea[i] = dim[i] = mul[i] = 0;
                                printf("\n> "); } /* blank line */
;

symex:  measure              { $$ = $1;         }
        | symex '*' symex    { $$ = $1;    }
        | symex '/' symex    { $$ = $1;    }
        | symex '^' numex    { $$ = $1; }
        | '(' symex ')'      { $$ = $2;         }
;

measure:   T_SI_prefix { $$ = $1; }
         | T_length    { $$ = $1; mea[0]++; }
         | T_mass      { $$ = $1; mea[1]++; }
         | T_time      { $$ = $1; mea[2]++; }
         | T_current   { $$ = $1; mea[3]++; }
         | T_temp      { $$ = $1; mea[4]++; }
         | T_lumi      { $$ = $1; mea[5]++; }
         | T_mole      { $$ = $1; mea[6]++; }
         | T_freq      { $$ = $1; mea[7]++; }
         | T_ang_rad   { $$ = $1; mea[8]++; }
         | T_ang_deg   { $$ = $1; mea[9]++; }
         | T_solid_ang { $$ = $1; mea[10]++; }
         | T_Jansky    { $$ = $1; mea[11]++; }
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


int main(int argc, char **argv) {

    int i;
    for (i=0; i<32; i++) mea[i] = dim[i] = mul[i] = 0; 

    yyparse();
    
    return 0;
}

void yyerror(char const *s) {
    fprintf(stderr, "error: %s\n", s);
}

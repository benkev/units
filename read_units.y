/* 
 * Parse an expression of measurement units
*/

%{

%}

/* Declare tokens */
%token NUM MEASURE

/* Declare precedence and associativity */
/* Operators are declared in increasing order of precedence */
%left '+' '-'
%left '*' '/'
%left NEG     /* negation - unary minus */
%left POS     /* unary plus             */
%right '^'    /* exponentiation         */

/* Grammar */
%%

exp:    MEASURE              { $$ = $1;         }
        | NUM                { $$ = $1;         }
        | NUM '+' NUM        { $$ = $1 + $3;    }
        | NUM '-' NUM        { $$ = $1 - $3;    }
        | '-' NUM  %prec NEG { $$ = -$2;        }
        | '+' NUM  %prec POS { $$ = $2;        }
        | exp '*' exp        { $$ = $1 * $3;    }
        | exp '/' exp        { $$ = $1 / $3;    }
        | exp '^' exp        { $$ = pow ($1, $3); }
        | '(' exp ')'        { $$ = $2;         }
;
%%


main(int argc, char **argv) {
    yyparse();
}

yyerror(char *s) {
    fprintf(stderr, "error: %s\n", s);
}

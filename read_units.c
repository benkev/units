#include <stdio.h>
#include <math.h>
#include "read_units.h"
#include "read_units.lex.h"
#include "read_units.tab.h"



int main(void) {

    char const meas_exp[] = "cm*((mg^2*us^-3/nA)^-5*K^5/cd/" \
        "(mole*THz)^3*day)^2*rad*Jy^(7+2*(4-6))";
    
    
    YY_BUFFER_STATE buf;

    buf = yy_scan_string(meas_exp);

    printf("Measurement expression:\n \"%s\"\n\n", meas_exp);

    int tok = 0;

    printf("Lexical analysis:\n");
    
    while(tok = yylex())
        switch (tok) {
        case T_symbol:
            printf("%s ", yylval.s); break;
        case T_number:
            printf("%d ", yylval.d); break;
        case '+': case '-': case '*': case '/':
        case '^': case '(': case ')':
            printf("'%c' ", tok); break;
        }
    printf("\n\n");
    

    yy_delete_buffer(buf);

    /*
     * Parse
     */
    buf = yy_scan_string(meas_exp);

    expr_list *el; /* Pointer to the list of measures with their exponents */
    
    yyparse(el);

    printf("The list of measures with their exponents found in expression:\n");
    printf("\n");
    print_list(el);
    printf("\n");
    
    yy_delete_buffer(buf);
    
    return 0;
}

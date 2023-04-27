#include <stdio.h>
#include <math.h>
#include "read_units.h"
#include "read_units.lex.h"
#include "read_units.tab.h"



int main(void) {

    char const meas_exp[] = "cm*((mg^2*us^-3/nA)^-5*K^5/cd/" \
        "(mole*THz)^3*day)^2*rad*Jy^(7+2*(4-6))";
    
    /* char const meas_exp[] = " A*kg*(m^-1*s^-2)^3"; */
        
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
    printf("\n\n====================================\n\n");
    printf("Parsing this expression as AST tree:\n\n");
    

    yy_delete_buffer(buf);

    /*
     * Parse
     */
    buf = yy_scan_string(meas_exp);

    expr_list *el; /* Pointer to the list of measures with their exponents */
    
    yyparse(el);
    
    printf("\n");
    printf("The list of measures with their exponents found in expression:\n");
    printf("\n");
    
    expr_list *el1 = el;
    printf("el=%p, el1=%p\n", el, el1);
    print_list(el);
    if (el == el1) printf("el == el1\n");
    if (el != el1) printf("el != el1\n");
    printf("el=%p, el1=%p\n", el, el1);

    //print_list(el);

    printf("\n");
    
    yy_delete_buffer(buf);
  
    return 0;
}

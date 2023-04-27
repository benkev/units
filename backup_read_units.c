#include <stdio.h>
#include <math.h>
#include "read_units.h"
#include "read_units.lex.h"
#include "read_units.tab.h"

/*
 * Table of measurement units
 */
/* char const *const meas_tab[NMEAS] = \ */
/*  {"m", "kg", "s", "A", "K", "cd", "mol", "Hz", "rad", "deg", "sr", "Jy"}; */



int main(void) {

    char const meas_exp[] = "m*((kg^2*s^-3/A)^-5*K^5/cd/" \
        "(mol*Hz)^3*s)^2*rad*Jy^(7+2*(4-6))";
    
    /* char const meas_exp[] = " A*kg*(m^-1*s^-2)^3"; */

    /* char const meas_exp[] = " A * kg *(m^-1*s^-2)^3  "; */
        
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

    /* 
     * Pointer to the list (actually, itself a pointer to the head of the list
     * of measures with their exponents
     * 
     */
    expr_list *el; 

    /*
     * The value returned by yyparse is 0 if parsing was successful 
     *    (return is due to end-of-input).
     * The value is 1 if parsing failed because of invalid input, 
     *    i.e., input that contains a syntax error or that causes 
     *    YYABORT to be invoked.
     * The value is 2 if parsing failed due to memory exhaustion.
     *
     */

    int perr;
    if (perr = yyparse(&el)) return perr;  /* ======== ERROR ======== >>> */
    
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


    /*
     * Convert measurement expression from list form 
     * into array of measure powers
     */
    int mu;
    meas_pow mpow;
    explst_to_dims(el, &mpow);

    for (mu=0; mu<NMEAS; mu++) {
        if (mpow.mea[mu]) printf("%s^%d ", meas_tab[mu], mpow.dim[mu]);
    }
  
    return 0;
}

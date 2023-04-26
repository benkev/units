/*
 * Handwritten lexer for measurement units expressions
 */
#include <stdio.h>
#include <ctype.h>
#include "read_units.h"
//#include "read_units.tab.h"

#define T_number  259
#define T_symbol  261

union YYSTYPE {
    char *s;
    int d;
};
typedef union YYSTYPE YYSTYPE;

typedef struct {
    char *pchar;
    


char const str[] = \
    "cm*((mg^2*us^-3/nA)^-5*K^5/cd/(mole*THz)^3*day)^2*rad*Jy^(7+2*(4-6))";

    int yylex(YYSTYPE *yylvalp, const char str[], char *pchar, YYSTYPE *yylvalp) {

    char *pstr = str;
    
    while(1) {
        
        /* Get a number */
        if(isdigit(*pstr)) {
            int ic = *pstr++ - '0';
            while(isdigit(*pstr)) { /* Convert substring to int */
                ic = (10*ic) + (*pstr - '0');
            }
            yylvalp->d = ic;
            if (*pstr == '\0') seeneos = 1;
            else ungetc(c, yyin);
            return T_number;
        }
        
        /* Get a measurement unit */
        if (isalpha(*pstr)) { 
            int i = c - 'A';
            while(isalpha(c = getc(yyin)))
                i = (10*i) + c-'A';
            yylval = i;
            if(c == EOF) seeneof = 1;
            else ungetc(c, yyin);
            return T_symbol;
        }
        
        /* Arithmetic operators and parentheses */
        switch(c) {
        case '+': case '-': case '*': case '/':
        case '^': case '*': case '(': case ')': return c;
        case '*': return MUL; case '|': return ABS;
        case '(': return OP; case ')': return CP;
        case '\n': return EOL;
        case
            ' ': case '\t': break;
            /* ignore these */
        case
            EOF: return 0;
            /* standard end-of-file token */
        case '/': c = getc(yyin);
            if(c == '/') {
                /* it's a comment */
                while((c = getc(yyin)) != '\n')
                    if(c == EOF) return 0; /* EOF in comment line */
                break;
            }
            if(c == EOF) seeneof = 1; /* it's division */
            else ungetc(c, yyin);
            return DIV;
        default: yyerror("Mystery character %c\n", c); break;
        }
    }
}

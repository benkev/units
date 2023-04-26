%option noyywrap
%option header-file="read_units.lex.h"

%{
    
#include "read_units.h"
#include "read_units.tab.h"
    
%}


%%
 /* single character ops */
"+" |
"-" |
"*" |
"/" |
"^" |
"(" |
")"        { return yytext[0]; }
[0-9]+	   { yylval.d = atoi(yytext); return T_number; }
[a-zA-Z]+  { yylval.s = strdup(yytext); return T_symbol; }
[ \t]      { /* ignore white space */ }
.	       { printf("Mystery character %c\n", *yytext); }
%%



/* int main(void) { */

/*     char meas_exp[] = "cm*((mg^2*us^-3/nA)^-5*K^5/cd/(mole*THz)^3*day)^2*" \ */
/*         "rad*Jy^(7+2*(4-6))"; */
/*     int tok = 0; */
/*     YY_BUFFER_STATE buf; */

/*     buf = yy_scan_string(meas_exp); */

/*     printf("Measurement expression:\n \"%s\"\n\n", meas_exp); */
/*     printf("Lexical analysis:\n"); */
    
/*     while(tok = yylex()) */
/*         switch (tok) { */
/*         case T_symbol: */
/*             printf("%s ", yylval.s); break; */
/*         case T_number: */
/*             printf("%d ", yylval.d); break; */
/*         case '+': case '-': case '*': case '/': */
/*         case '^': case '(': case ')': */
/*             printf("'%c' ", tok); break; */
/*         } */
/*     printf("\n"); */
    
/*     yy_delete_buffer(buf); */
/*     return 0; */
/* } */



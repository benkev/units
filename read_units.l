%option noyywrap
/* %option reentrant */

%{
    
# include "read_units.tab.h"
    
%}

/* Definition (Substitution) Section */

D		[0-9]
INT		[+-]?{D}+
NUM		{INT}

SI_PREFIX    [ \t]*("q"|"r"|"y"|"z"|"a"|"f"|"p"|"n"|"u"|"m"|"c"|"d"|"da"|"h"|"k"|"M"|"G"|"T"|"P"|"E"|"Z"|"Y"|"R"|"Q"|"quecto"|"ronto"|"yocto"|"zepto"|"atto"|"femto"|"pico"|"nano"|"micro"|"milli"|"centi"|"deci"|"deka"|"hecto"|"kilo"|"Mega"|"Giga"|"Tera"|"Peta"|"Exa"|"Zetta"|"Yotta"|"Ronna"|"Quetta")
    
PLAINANGLE	[ \t]*("\""|"''"|"'"|"deg"|"rad")
SOLIDANGLE	[ \t]*"sr"
FREQ	[ \t]*("mHz"|"Hz"|"kHz"|"MHz"|"GHz")
DUR		[ \t]*("us"|"usec"|"ms"|"msec"|"s"|"sec"|"min"|"hr")
LENGTH  [ \t]*("pm"|"nm"|"um"|"mm"|"cm"|"dm"|"m"|"dam"|"hm"|"km"|"Mm"|"Gm")
MASS    [ \t]*("pg"|"ng"|"ug"|"mg"|"cg"|"dg"|"g"|"dag"|"hg"|"kg"|"Mg"|"Gg")


%%
 /* single character ops */
"+" |
"-" |
"*" |
"/" |
"^" |
"(" |
")"         { return yytext[0]; }
[0-9]+	    { yylval.d = atoi(yytext); return NUM; }
[a-zA-z]+   { yylval.s = strdup(yytext); return MEASURE; }
[ \t]   { /* ignore white space */ }
.	{ printf("Mystery character %c\n", *yytext); }

%%


/* int main(void) */
/* { */
/*     int tok; */
    
/*     yyscan_t scanner; */
/*     YY_BUFFER_STATE buf; */
/*     yylex_init(&scanner); */
/*     buf = yy_scan_string("m^(2+3*(7^-3))", scanner); */

/*     while(tok = yylex(scanner)) { */
/*          if (tok == NUM) */
/*              printf("%d ", yylval); */
/*          else */
/*              printf("%c ", tok); */

/*     } */
/*     printf("\n"); */
    
/*     yy_delete_buffer(buf, scanner); */
/*     yylex_destroy(scanner); */
/*     return 0; */
/* } */



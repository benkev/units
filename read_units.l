%option noyywrap
/* %option reentrant */

%{
    
# include "read_units.tab.h"
    
%}

/* Definition (Substitution) Section */

/* SI_PREFIX    [ \t]*("q"|"r"|"y"|"z"|"a"|"f"|"p"|"n"|"u"|"m"|"c"|"d"|"da"|"h"|"k"|"M"|"G"|"T"|"P"|"E"|"Z"|"Y"|"R"|"Q"|"quecto"|"ronto"|"yocto"|"zepto"|"atto"|"femto"|"pico"|"nano"|"micro"|"milli"|"centi"|"deci"|"deka"|"hecto"|"kilo"|"Mega"|"Giga"|"Tera"|"Peta"|"Exa"|"Zetta"|"Yotta"|"Ronna"|"Quetta") */

SI_PREFIX    [ \t]*("quecto"|"ronto"|"yocto"|"zepto"|"atto"|"femto"|"pico"|"nano"|"micro"|"milli"|"centi"|"deci"|"deka"|"hecto"|"kilo"|"Mega"|"Giga"|"Tera"|"Peta"|"Exa"|"Zetta"|"Yotta"|"Ronna"|"Quetta")
    
LENGTH  [ \t]*("pm"|"nm"|"um"|"mm"|"cm"|"dm"|"m"|"dam"|"hm"|"km"|"Mm"|"Gm")
MASS    [ \t]*("pg"|"ng"|"ug"|"mg"|"cg"|"dg"|"g"|"kg"|"T"|"kT"|"MT"|"GT")
TIME	[ \t]*("fs"|"ns"|"us"|"usec"|"ms"|"msec"|"s"|"sec"|"min"|"hr"|"day")
CURRENT   [ \t]*("fA"|"pA"|"nA"|"uA"|"mA"|"A"|"kA"|"MA"|"MegA")
TEMP    [ \t]*"K"
LUMI    [ \t]*"cd"
MOLE    [ \t]*"mole"
FREQ	[ \t]*("mHz"|"Hz"|"kHz"|"MHz"|"GHz"|"THz")
ANG_RAD     [ \t]*("rad")
ANG_DEG     [ \t]*("\""|"''"|"'"|"deg")
SOLID_ANG	[ \t]*"sr"
JANSKY	[ \t]*"Jy"

%%
 /* single character ops */
"+" |
"-" |
"*" |
"/" |
"^" |
"(" |
")"   { return yytext[0]; }
[0-9]+	     { yylval.d = atoi(yytext); return T_number; }
{LENGTH}     { yylval.s = strdup(yytext); return T_length; }
{MASS}       { yylval.s = strdup(yytext); return T_mass; }
{TIME}       { yylval.s = strdup(yytext); return T_time; }
{CURRENT}    { yylval.s = strdup(yytext); return T_current; }
{TEMP}       { yylval.s = strdup(yytext); return T_temp; }
{LUMI}       { yylval.s = strdup(yytext); return T_lumi; }
{MOLE}       { yylval.s = strdup(yytext); return T_mole; }
{FREQ}       { yylval.s = strdup(yytext); return T_freq; }
{ANG_RAD}    { yylval.s = strdup(yytext); return T_ang_rad; }
{ANG_DEG}    { yylval.s = strdup(yytext); return T_ang_deg; }
{SOLID_ANG}  { yylval.s = strdup(yytext); return T_solid_ang; }
{JANSKY}     { yylval.s = strdup(yytext); return T_Jansky; }
{SI_PREFIX}  { yylval.s = strdup(yytext); return T_SI_prefix; }
\n      { return EOL; }
[ \t]   { /* ignore white space */ }
.	{ printf("Mystery character %c\n", *yytext); }
%%

/* [a-zA-Z]+   { yylval.s = strdup(yytext); return MEASURE; } */


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



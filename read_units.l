%option noyywrap

%{

#include <stdio.h>
#include <string.h> 








%}

%x Definition (Substitution) Section

D		[0-9]
INT		[+-]?{D}+
NUM		{INT}
ANGLE	[ \t]*("\""|"''"|"'"|"deg"|"rad")
FREQ	[ \t]*("mHz"|"Hz"|"kHz"|"MHz"|"GHz")
DUR		[ \t]*("us"|"usec"|"ms"|"msec"|"s"|"sec"|"min"|"hr")



%%





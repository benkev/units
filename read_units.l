%option noyywrap

%{

#include <stdio.h>
#include <string.h> 








%}

/* Definition (Substitution) Section */

D		[0-9]
INT		[+-]?{D}+
NUM		{INT}
PLAINANGLE	[ \t]*("\""|"''"|"'"|"deg"|"rad")
SOLIDANGLE	[ \t]*"sr"
FREQ	[ \t]*("mHz"|"Hz"|"kHz"|"MHz"|"GHz")
DUR		[ \t]*("us"|"usec"|"ms"|"msec"|"s"|"sec"|"min"|"hr")



%%





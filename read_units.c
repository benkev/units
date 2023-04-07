#include <stdio.h>
#include "read_units.h"
#include "read_units.tab.h"

int main(int argc, char **argv) {
       
    yyparse();

    printf("Done.\n");
    return 0;
}

 

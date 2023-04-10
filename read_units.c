#include <stdio.h>
#include "read_units.h"
#include "read_units.tab.h"

int main(int argc, char **argv) {
    
    yyscan_t scanner;
    
    yylex_init(&scanner);
    yyset_in(fopen(argv[1], "rb"), scanner);
    yylex(scanner);
    yylex_destroy(scanner);
    
    yyparse();

    printf("Done.\n");
    return 0;
}

 

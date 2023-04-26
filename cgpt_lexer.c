#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#define T_number  259
#define T_symbol  261

union YYSTYPE {
    char *s;
    int d;
};
typedef union YYSTYPE YYSTYPE;

/*
 * My initial header:
 * int yylex(const char str[], char *pchar, YYSTYPE *yylvalp);
 */

int yylex(const char str[], char **pchar, YYSTYPE *yylvalp) {
    const char *start;
    char *end;

    while (isspace(**pchar)) {
        (*pchar)++;
    }

    if (**pchar == '\0') {
        return 0;
    }

    if (strchr("+-*/^()", **pchar) != NULL) {
        yylvalp->s = (char *) malloc(2 * sizeof(char));
        yylvalp->s[0] = **pchar;
        yylvalp->s[1] = '\0';
        (*pchar)++;
        return **pchar;
    }

    if (isdigit(**pchar)) {
        yylvalp->d = strtol(*pchar, &end, 10);
        *pchar = end;
        return T_number;
    }

    if (isalpha(**pchar)) {
        start = *pchar;
        while (isalnum(**pchar) || **pchar == '_') {
            (*pchar)++;
        }
        int len = *pchar - start;
        yylvalp->s = (char *) malloc((len + 1) * sizeof(char));
        strncpy(yylvalp->s, start, len);
        yylvalp->s[len] = '\0';
        return T_symbol;
    }

    return -1;
}

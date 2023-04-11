    /* Lexer */
%{
    #include "nasi.tab.h"
    #include <stdlib.h>
    #include <string>
    #define YYSTYPE bool
    void yyerror (yyscan_t yyscanner, char const *msg);
%}

%option reentrant bison-bridge

%%

[0-1]      {
                if (strcmp(yytext, "0")==0)
                {
                    *yylval = false;
                }
                else
                {
                    *yylval = true;
                }

                //yylval = atoi(yytext);
                return INTEGER;
            }

[&|!()\n]     { return *yytext; }

[ \t]   ;       /* skip whitespace */

.               yyerror (yyscanner, "Unknown character");

%%

int yywrap(yyscan_t yyscanner)
{
    return 1;
}

bool parseExpression(const std::string& inp)
{
    yyscan_t myscanner;
    yylex_init(&myscanner);
    struct yyguts_t * yyg = (struct yyguts_t*)myscanner;

    yy_delete_buffer(YY_CURRENT_BUFFER,myscanner);

    /*Copy string into new buffer and Switch buffers*/
    yy_scan_string(inp.c_str(), myscanner);

    bool nasi = yyparse(myscanner);
    yylex_destroy(myscanner);
    return nasi;
}




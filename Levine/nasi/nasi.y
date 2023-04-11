%{
    #include <stdio.h>
    #include <string>

    #define YYSTYPE bool
    typedef void* yyscan_t;
    void yyerror (yyscan_t yyscanner, char const *msg);
    int yylex(YYSTYPE *yylval_param, yyscan_t yyscanner);
    bool parseExpression(const std::string& inp);
%}


%define api.pure full
%lex-param {yyscan_t scanner}
%parse-param {yyscan_t scanner}

%token INTEGER
%left '&' '|'

%%

program:
        program statement '\n'
        | /* NULL */
        ;

statement:
        expression                      { printf("%d\n", $1); return $1; }
        ;

expression:
        INTEGER
        | expression '|' expression     { $$ = $1 | $3; }
        | expression '&' expression     { $$ = $1 & $3; }
        | '(' expression ')'            { $$ = $2; }
        | '!' expression                { $$ = !$2; }
        ;

%%

void yyerror (yyscan_t yyscanner, char const *msg){
    fprintf(stderr, "%s\n", msg);
}


int main(void) {

    std::string inp = "1|0\n";

    bool nasi = parseExpression(inp);
    printf("%s%d\n", "nasi ", nasi);
    printf("Press ENTER to close. ");
    getchar();
}


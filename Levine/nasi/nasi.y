/* 
 * Parser
 */

%{
    #include <stdio.h>
    #include <string>
    #include <iostream>

    /* #include <print> */

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
%right '!'

%%

program:
        program statement '\n'
        | /* NULL */
        ;

statement:
        expression                      { /* printf("%d\n", $1); */ return $1; }
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

    std::string inp = "1|(0&1|0)\n";

    bool nasi = parseExpression(inp);
    std::cout << inp.substr(0, inp.size()-1) << " == " << nasi
              << std::endl;
    /* std::print("{} => {}\n", inp, nasi); */
    /* printf("Press ENTER to close. "); */
    /* getchar(); */
}


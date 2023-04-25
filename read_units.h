/*
 * Declarations for read_units reentrant lexer and parser
 */
#include "read_units.tab.h"
#include "read_units.lex.h"

typedef unsigned char uchar;

/* interface to the lexer */
/* extern int yylineno; /\* from lexer *\/ */
void yyerror(char *s, ...);

/* Tree node in the Abstract Syntax Tree */
typedef struct ast_node {
  int nodetype;
  struct ast_node *l;
  struct ast_node *r;
} ast_node;

/* Tree leaf with a number value */
typedef struct num_leaf {
  int nodetype;			/* type K */
  int number;
} num_leaf;

/* Tree leaf with a measure value */
typedef struct meas_leaf {
  int nodetype;			/* type M  */
  int measure;
} meas_leaf;

/* Element of list with a measure value */
typedef struct expr_list {
  int measure;
  int power;
  struct expr_list *next;
} expr_list;

/* Per-parse data */
typedef struct pcdata {
    yyscan_t scanner;   /* Scanner context */
    expr_list *el;      /* List of found measures with powers */
} pcdata;

/* typedef union umea { */
/*     int mea; */
/*     struct { */
/*         uchar mul; */
/*         char imea; */
/*     }; */
/* } umea; */

/* build an AST */
ast_node *newast(int nodetype, ast_node *l, ast_node *r);
ast_node *newnum(int d);
ast_node *newmeas(int measure);
expr_list *newexpr(int measure, int power, expr_list *next);
expr_list *concat(expr_list *const expl, expr_list *const expr);
void mulpwr(expr_list *const exp, int pwr);
int getmeas(char const *sym);
void print_tree(ast_node *a);
void print_list(expr_list *const expr);
                
/* Reduce an AST */
expr_list *reduce(ast_node *a, expr_list *head);

/* delete and free an AST */
void treefree(ast_node *);


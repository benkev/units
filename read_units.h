/*
 * Declarations for read_units lexer/parser
 */

/* interface to the lexer */
extern int yylineno; /* from lexer */
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
typedef struct measure_leaf {
  int nodetype;			/* type K */
  int measure;
  int power;
  int prefix;
} measure_leaf;

/* Element of list with a measure value */
typedef struct expr_list {
  int measure;
  int power;
  int prefix;
  struct expr_list *next;
} expr_list;

/* build an AST */
ast_node *newast(int nodetype, ast_node *l, ast_node *r);
ast_node *newnum(int d);
ast_node *newmeas(int measure, int power, int prefix);
expr_list *newexpr(int measure, int power, int prefix, expr_list *next);

/* Reduce an AST */
int reduce(ast_node *);

/* delete and free an AST */
void treefree(ast_node *);


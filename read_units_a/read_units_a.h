/*
 * Declarations for read_units lexer/parser
 */

/* /\* */
/*  * Table of measurement units */
/*  *\/ */
/* #define NMEAS 13 */
/* #define NMODS 13 */

/* char const *const meas_tab[NMEAS][NMODS] = { */
/*     {"pm","nm","um","mm","cm","dm","m","dam","hm","km","Mm","Gm",0}, */
/*     {"pg","ng","ug","mg","cg","dg","g","kg","T","kT","MT","GT",0}, */
/*     {"fs","ps","ns","us","usec","ms","msec","s","sec",0}, */
/*     {"min","hr","day","mon","yr",0}, */
/*     {"fA","pA","nA","uA","mA","A","kA","MA","MegA",0,0,0,0}, */
/*     {"K",0,0,0,0,0,0,0,0,0,0,0,0}, */
/*     {"cd",0,0,0,0,0,0,0,0,0,0,0,0}, */
/*     {"mole",0,0,0,0,0,0,0,0,0,0,0,0}, */
/*     {"mHz","Hz","kHz","MHz","GHz","THz",0,0,0,0,0,0,0}, */
/*     {"rad",0,0,0,0,0,0,0,0,0,0,0,0}, */
/*     {"\"","''","'","deg",0,0,0,0,0,0,0,0,0}, */
/*     {"sr",0,0,0,0,0,0,0,0,0,0,0,0}, */
/*     {"Jy",0,0,0,0,0,0,0,0,0,0,0,0}, */
/* }; */

/* /\* */
/*  * Table of multipliers contains decimal powers, like -3 for "milli" etc. */
/*  *\/ */
/* char const mul_tab[NMEAS][NMODS] = { */
/*     {-12,-9,-6,-3,-2,-1,0,1,2,3,6,9,127}, */
/*     {-12,-9,-6,-3,-2,-1,0,3,6,9,12,15,127}, */
/*     {-15,-12,-9,-6,-6,-3,-3,127,127}, */
/*     {0,0,0,0,0,127,127,127,127,127,127,127}, */
/*     {-15,-12,-9,-6,-3,0,3,6,6,127,127,127,127}, */
/*     {0,127,127,127,127,127,127,127,127,127,127,127,127}, */
/*     {0,127,127,127,127,127,127,127,127,127,127,127,127}, */
/*     {0,127,127,127,127,127,127,127,127,127,127,127,127}, */
/*     {-3,0,3,6,9,12,127,127,127,127,127,127,127}, */
/*     {0,127,127,127,127,127,127,127,127,127,127,127,127}, */
/*     {0,0,0,0,127,127,127,127,127,127,127,127,127}, */
/*     {0,127,127,127,127,127,127,127,127,127,127,127,127}, */
/*     {0,127,127,127,127,127,127,127,127,127,127,127,127}, */
/* }; */


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

/* build an AST */
ast_node *newast(int nodetype, ast_node *l, ast_node *r);
ast_node *newnum(int d);
ast_node *newmeas(int measure);
expr_list *newexpr(int measure, int power, expr_list *next);
expr_list *concat(expr_list *const expl, expr_list *const expr);
void mulpwr(expr_list *const exp, int pwr);
int getmeas(char const *sym);
void print_tree(ast_node *a);

/* Reduce an AST */
expr_list *reduce(ast_node *);

/* delete and free an AST */
void treefree(ast_node *);


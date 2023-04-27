/*
 * Declarations for read_units lexer/parser
 */

#define NMEAS 12

typedef unsigned char uchar;

enum measure_index {i_length = 0, i_mass, i_time, i_current,
                    i_temp, i_lumi, i_mole, i_freq, i_ang_rad, i_ang_deg,
                    i_solid_ang, i_Jansky};

extern char const *const meas_tab[NMEAS]; /* Table of measurement units */

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

/* Positional representation of measure expression */
typedef struct meas_pow {
    int dim[NMEAS];  /* powers of the units */
    int mea[NMEAS];  /* 1: a measure present, 0 - absent */
} meas_pow;

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

/* Convert measurement expression from list into array of measure powers */
void explst_to_dims(expr_list *explst, meas_pow *mpow);

/* interface to the lexer */
extern int yylineno; /* from lexer */
void yyerror(expr_list **el, char *s, ...);


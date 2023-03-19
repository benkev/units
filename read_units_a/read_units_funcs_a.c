/*
 * Parser functions for 
 */
#  include <stdio.h>
#  include <stdlib.h>
#  include <stdarg.h>
#  include "read_units_a.h"

extern expr_list *globexpr;

int yyparse (void);

ast_node *
newast(int nodetype, ast_node *l, ast_node *r)
{
  ast_node *a = malloc(sizeof(ast_node));
  
  if(!a) {
    yyerror("out of space");
    exit(0);
  }
  a->nodetype = nodetype;
  a->l = l;
  a->r = r;
  return a;
}

ast_node *
newnum(int d)
{
  num_leaf *a = malloc(sizeof(num_leaf));
  
  if(!a) {
    yyerror("out of space");
    exit(0);
  }
  a->nodetype = 'K';
  a->number = d;
  return (ast_node *)a;
}

ast_node *
newmeas(int measure)
{
  meas_leaf *a = malloc(sizeof(meas_leaf));
  
  if(!a) {
    yyerror("out of space");
    exit(0);
  }
  a->nodetype = 'M';
  a->number = d;
  return (ast_node *)a;
}

expr_list *
newexpr(int measure, int power, expr_list *next)
{
  expr_list *a = malloc(sizeof(expr_list));
  
  if(!a) {
    yyerror("out of space");
    exit(0);
  }
  a->nodetype = 'K';
  a->number = d;
  return (ast_node *)a;
}

expr_list *
reduce(ast_node *a)
{
  int pwr;
  expr_list *exp, *expl, *expr;

  switch(a->nodetype) {
  case '^': {
      pwr = (a->r)->number;
      expl = reduce(a->l);  /* Either just a measure, or list with powers  */
      if (expl->nodetype == 'M') {
          exp = newexpr(expl->measure, pwr, globexpr);
          return exp;
      }
      else { /* Multiply powers of every list item by pwr */
          
      }
      break;
  }
      
  case 'K': v = ((num_leaf *)a)->number; break;

  case '*': v = eval(a->l) * eval(a->r); break;
  case '/': v = eval(a->l) / eval(a->r); break;
  case 'M': v = -eval(a->l); break;
  default: printf("internal error: bad node %c\n", a->nodetype);
  }
  return v;
}

void
treefree(ast_node *a)
{
  switch(a->nodetype) {

    /* two subtrees */
  case '+':
  case '-':
  case '*':
  case '/':
    treefree(a->r);

    /* one subtree */
  case '|':
  case 'M':
    treefree(a->l);

    /* no subtree */
  case 'K':
    free(a);
    break;

  default: printf("internal error: free bad node %c\n", a->nodetype);
  }
}

void
yyerror(char *s, ...)
{
  va_list ap;
  va_start(ap, s);

  fprintf(stderr, "%d: error: ", yylineno);
  vfprintf(stderr, s, ap);
  fprintf(stderr, "\n");
}

int
main()
{
  printf("> "); 
  return yyparse();
}

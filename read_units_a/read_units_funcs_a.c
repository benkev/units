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
  a->measure = measure;
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
  a->measure = measure;
  a->power = power;
  a->next = next;
  return a;
}

expr_list *
reduce(ast_node *a) {
    int pwr, meas;
    num_leaf *numleaf;
    meas_leaf *measleaf;
    expr_list *exp, *expl, *expr;
    ast_node *nodl, *nodr;

    switch(a->nodetype) {
    case '^': {
        numleaf = (num_leaf *) a->r;
        if (numleaf->nodetype != 'K') {
            printf("Error: non-numeric power exponent, node type  %c\n",
                   numleaf->nodetype);
            return 0;
        }
        pwr = numleaf->number;

        nodl = a->l;
        if (nodl->nodetype == 'M') {
            measleaf = (meas_leaf *) nodl;
            exp = newexpr(measleaf->measure, pwr, 0);
        }
        else {
            exp = reduce(nodl);
            /* Multiply powers of every list item by pwr */
            mulpwr(exp, pwr);
        }
        break;
    }
      
    case '*': {
        nodl = a->l;
        if (nodl->nodetype == 'M') {
            measleaf = (meas_leaf *) nodl;
            expl = newexpr(measleaf->measure, 1, 0);
        }
        else
            expl = reduce(nodl);
        
        nodr = a->r;
        if (nodr->nodetype == 'M') {
            measleaf = (meas_leaf *) nodr;
            expr = newexpr(measleaf->measure, 1, 0);
        }
        else
            expr = reduce(nodr);

        exp = concat(expl, expr);
        
        break;
    }
      
    case '/': {
        nodl = a->l;
        if (nodl->nodetype == 'M') {
            measleaf = (meas_leaf *) nodl;
            expl = newexpr(measleaf->measure, 1, 0);
        }
        else
            expl = reduce(nodl);
        
        nodr = a->r;
        if (nodr->nodetype == 'M') {
            measleaf = (meas_leaf *) nodr;
            expr = newexpr(measleaf->measure, -1, 0);
        }
        else {
            expr = reduce(nodr);
            /* Multiply powers of every list item by -1 */
            mulpwr(expr, -1);
        }
        exp = concat(expl, expr);
        
        break;
    }

    default: printf("internal error: bad node %c\n", a->nodetype);
    }
    
    return exp;
}


void
treefree(ast_node *a)
{
  switch(a->nodetype) {

    /* two subtrees */
  case '*':
  case '/':
    treefree(a->r);

    /* one subtree */
  case '^':
    treefree(a->l);

    /* no subtree */
  case 'M':
  case 'K':
    free(a);
    break;

  default: printf("internal error: free bad node %c\n", a->nodetype);
  }
}


/* 
 * Multiply powers of every list item by pwr 
 */
void mulpwr(expr_list *const exp, int pwr) {

    expr_list *ep = exp;
    while (ep->next) {
        ep->power *= pwr;
        ep = ep->next;
    }
}

/*
 * Join lists expl and expr
 */
expr_list *concat(expr_list *const expl, expr_list *const expr) {

    expr_list *ep = expl;
    while (ep->next)
        ep = ep->next;
    ep->next = expr; /* Point the last expl item at the first item of expr */ 
    
    return expl;
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

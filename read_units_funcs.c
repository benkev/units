/*
 * Parser functions for 
 */
#  include <stdio.h>
#  include <stdlib.h>
#  include <stdarg.h>
#  include <string.h>
#  include "read_units.h"

expr_list *explst;

/*
 * Table of measurement units
 */
char const *const meas_tab[NMEAS] = \
    {"m", "kg", "s", "A", "K", "cd", "mol", "Hz", "rad", "deg", "sr", "Jy"};


ast_node *
newast(int nodetype, ast_node *l, ast_node *r)
{
  ast_node *a = malloc(sizeof(ast_node));
  
  if(!a) {
    yyerror(&explst, "out of space");
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
    yyerror(&explst, "out of space");
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
    yyerror(&explst, "out of space");
    exit(0);
  }
  a->nodetype = 'M';
  a->measure = measure;
  return (ast_node *)a;
}


expr_list *
newexpr(int measure, int power, expr_list *next)
{
  expr_list *ep = malloc(sizeof(expr_list));
  
  if(!ep) {
    yyerror(&explst, "out of space");
    exit(0);
  }

  /* int imea = 0x0ff & measure; */
  /* printf("newexpr: ep = %p, next = %p, '%s'\n", ep, next, measures[imea]); */
  
  ep->measure = measure;
  ep->power = power;
  ep->next = next;
  return ep;
}


expr_list *
reduce(ast_node *a, expr_list *head) {
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
            exp = newexpr(measleaf->measure, pwr, head);
        }
        else {
            exp = reduce(nodl, head);
            /* Multiply powers of every list item by pwr */
            mulpwr(exp, pwr);
        }
        break;
    }
      
    case '*': {
        nodl = a->l;
        if (nodl->nodetype == 'M') {
            measleaf = (meas_leaf *) nodl;
            expl = newexpr(measleaf->measure, 1, head);
        }
        else
            expl = reduce(nodl, head);
        
        nodr = a->r;
        if (nodr->nodetype == 'M') {
            measleaf = (meas_leaf *) nodr;
            expr = newexpr(measleaf->measure, 1, head);
        }
        else
            expr = reduce(nodr, head);

        exp = concat(expl, expr);
        
        break;
    }
      
    case '/': {
        nodl = a->l;
        if (nodl->nodetype == 'M') {
            measleaf = (meas_leaf *) nodl;
            expl = newexpr(measleaf->measure, 1, head);
        }
        else
            expl = reduce(nodl, head);
        
        nodr = a->r;
        if (nodr->nodetype == 'M') {
            measleaf = (meas_leaf *) nodr;
            expr = newexpr(measleaf->measure, -1, head);
        }
        else {
            expr = reduce(nodr, head);
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
    while (ep) {
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

/*
 * Lookup the table of measurement units meas_tab to find the measure index
 * from its locations. 
 * For example, 'Hz' is at [8], so 8 denotes 'Frequency in Hz'. 
 * Returns the measure index found.
 * If the measurement unit sym is not found in meas_tab, -1 is returned.
 */
int getmeas(char const *sym) {

    int i, streq;
    for (i=0; i<NMEAS; i++) {
        streq = strcmp(meas_tab[i], sym);
        if (streq == 0) break;
    }
    if (streq == 0) {
        printf("\n'%s': i=%d\n", sym, i);
        return  i;
    }
    else
        return -1;
}


void print_list(expr_list *const expr) {
    
    expr_list *ep = expr;
    int mu;
    while (ep) {
        mu = ep->measure;
        printf("%s^%d\n", meas_tab[mu], ep->power);
        ep = ep->next;
    }
}


void print_tree(ast_node *a) {

    switch(a->nodetype) {
        /* two subtrees */
    case '*':
    case '/':
    case '^':
        printf("'%c' ==> ", a->nodetype);
        print_tree(a->r);
        print_tree(a->l);
        break;

        /* no subtree */
    case 'K': {
        num_leaf *nl = (num_leaf *)a;
        printf("'%d'\n", nl->number);
        break;
    }
    case 'M': {
        meas_leaf *ml = (meas_leaf *)a;
        int mu = ml->measure;
        printf("'%s'\n", meas_tab[mu]);
        break;
    }
    default: printf("internal error: free bad node %c\n", a->nodetype);
    }
    return;
}


void
yyerror(expr_list **explst, char *s, ...)
{
  va_list ap;
  va_start(ap, s);

  /* fprintf(stderr, "%d: error: ", yylineno); */
  fprintf(stderr, "Error: ");
  vfprintf(stderr, s, ap);
  fprintf(stderr, "\n");
}


/*
 * Convert measurement expression from list form into array of measure powers
 *
 */
void explst_to_dims(expr_list *explst, meas_pow *mpow) {

    expr_list *ep = explst;
    int mu, pw;

    for (mu=0; mu<NMEAS; mu++) {
        mpow->dim[mu] = 0;
        mpow->mea[mu] = 0;  /* Assume no measures (empty expression) */
    }

    while (ep) {
        mu = ep->measure; /* Position in the array of measure powers */
        pw = ep->power;
        mpow->dim[mu] += pw;
        mpow->mea[mu] = 1;
        ep = ep->next;
    }
}

/*
 * Parser functions for 
 */
#  include <stdio.h>
#  include <stdlib.h>
#  include <stdarg.h>
#  include <string.h>
#  include "read_units_a.h"

/*
 * Table of measurement units
 */
#define NMEAS 13
#define NMODS 13


char const *const measures[NMEAS] = {"m", "g", "s", "day", "A", "K", "cd", \
    "mole", "Hz", "rad", "deg", "sr", "Jy"};

char const *const meas_tab[NMEAS][NMODS] = {
    {"pm","nm","um","mm","cm","dm","m","dam","hm","km","Mm","Gm",0},
    {"pg","ng","ug","mg","cg","dg","g","kg","T","kT","MT","GT",0},
    {"fs","ps","ns","us","usec","ms","msec","s","sec",0},
    {"min","hr","day","mon","yr",0},
    {"fA","pA","nA","uA","mA","A","kA","MA","MegA",0,0,0,0},
    {"K",0,0,0,0,0,0,0,0,0,0,0,0},
    {"cd",0,0,0,0,0,0,0,0,0,0,0,0},
    {"mole",0,0,0,0,0,0,0,0,0,0,0,0},
    {"mHz","Hz","kHz","MHz","GHz","THz",0,0,0,0,0,0,0},
    {"rad",0,0,0,0,0,0,0,0,0,0,0,0},
    {"\"","''","'","deg",0,0,0,0,0,0,0,0,0},
    {"sr",0,0,0,0,0,0,0,0,0,0,0,0},
    {"Jy",0,0,0,0,0,0,0,0,0,0,0,0},
};

/*
 * Table of multipliers contains decimal powers, like -3 for "milli" etc.
 */
char const mul_tab[NMEAS][NMODS] = {
    {-12,-9,-6,-3,-2,-1,0,1,2,3,6,9,127},
    {-12,-9,-6,-3,-2,-1,0,3,6,9,12,15,127},
    {-15,-12,-9,-6,-6,-3,-3,127,127},
    {0,0,0,0,0,127,127,127,127,127,127,127},
    {-15,-12,-9,-6,-3,0,3,6,6,127,127,127,127},
    {0,127,127,127,127,127,127,127,127,127,127,127,127},
    {0,127,127,127,127,127,127,127,127,127,127,127,127},
    {0,127,127,127,127,127,127,127,127,127,127,127,127},
    {-3,0,3,6,9,12,127,127,127,127,127,127,127},
    {0,127,127,127,127,127,127,127,127,127,127,127,127},
    {0,0,0,0,127,127,127,127,127,127,127,127,127},
    {0,127,127,127,127,127,127,127,127,127,127,127,127},
    {0,127,127,127,127,127,127,127,127,127,127,127,127},
};



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

/*
 * Lookup the table of measurement units meas_tab to find the measure index
 * from its row and column locations. 
 * For example, 'GHz' is at [8,4]. The row, 8, denotes 'Frequency in Hz'. 
 * Use the indices found to get the unit multiplier mul
 * in mul_tab. For example, mul_tab[8][4] is 9, so GHz is Hzx10^9. 
 * Return the combined measure index as an integer row*256 + mul (or, the same,
 * row<<8 + mul).
 * If the measurement unit sym is not found in meas_tab, -1 is returned.
 */
int getmeas(char const *sym) {

    int i, j, streq;
    for (i=0; i<NMEAS; i++) {
        j = 0;
        while (meas_tab[i][j] != 0) {
            streq = strcmp(meas_tab[i][j], sym);
            if (streq == 0) break;
            j++;
        }
        if (streq == 0) break;
    }
    if (streq == 0) {
        /* printf("'%s': i=%d, j=%d, mul_tab[i][j]=%d, return: %d\n",sym,i,j, */
        /*        (int) mul_tab[i][j], (i<<8) + (int) mul_tab[i][j]);  */
        return (i<<8) + (int) mul_tab[i][j];
    }
    else
        return -1;

}


void print_tree(ast_node *a) {

    switch(a->nodetype) {
    /* two subtrees */
    case '*':
    case '/':
        printf("Node '%c'\n", a->nodetype);
         printf("Right subtree:\n");
        print_tree(a->r);

    /* one subtree */
    case '^':
        printf("Node '%c'\n", a->nodetype);
        printf("Left subtree:\n");
        print_tree(a->l);

    /* no subtree */
    case 'K': {
        num_leaf *nl = (num_leaf *)a;
        printf("Leaf '%c': Number %d\n", a->nodetype, nl->number);
        break;
    }
    case 'M': {
        meas_leaf *ml = (meas_leaf *)a;
        int mea = ml->measure;
        int mu = mea/256, mul = mea%256;
        printf("Leaf '%c': Measure 0x%08x: %s x 10^%d\n", a->nodetype, mea,
               measures[mu], mul);
        break;
    }
    default: printf("internal error: free bad node %c\n", a->nodetype);
    }
    return;
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

/* int */
/* main() */
/* { */
/*   printf("> ");  */
/*   return yyparse(); */
/* } */

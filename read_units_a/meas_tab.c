/*
 * Trying measurement table
 *
 * gcc meas_tab.c -o mt
 */


#include <stdio.h>
#include <string.h>

/*
 * Table of measurement units
 */
#define NMEAS 13
#define NMODS 13

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


int main(int argc, char *argv[]) {

    int i, j;
    
        
    for (int i=0; i<NMEAS; i++) {
        j = 0;
        while (meas_tab[i][j] != 0) {
            printf("%s ", meas_tab[i][j]);
            j++;
        }
        printf("\n");
    }

    char *sym;
    sym = strdup("uA");
    
    for (i=0; i<NMEAS; i++) {
        j = 0;
        int streq;
        while (meas_tab[i][j] != 0) {
            streq = strcmp(meas_tab[i][j], sym);
            if (streq == 0) break;
            j++;
        }
        if (streq == 0) break;
    }
    printf("Found sym = '%s' at [%d,%d]\n", sym, i, j);

    sym = strdup("GHz");
    
    for (i=0; i<NMEAS; i++) {
        j = 0;
        int streq;
        while (meas_tab[i][j] != 0) {
            streq = strcmp(meas_tab[i][j], sym);
            if (streq == 0) break;
            j++;
        }
        if (streq == 0) break;
    }
    printf("Found sym = '%s' at [%d,%d]\n", sym, i, j);

    return 0;
}

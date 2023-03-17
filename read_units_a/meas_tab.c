#include <stdio.h>
#include <string.h>



char *meas_tab[12][13] = {
    {"pm","nm","um","mm","cm","dm","m","dam","hm","km","Mm","Gm",0},
    {"pg","ng","ug","mg","cg","dg","g","kg","T","kT","MT","GT",0},
    {"fs","ns","us","usec","ms","msec","s","sec","min","hr","day",0,0},
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

/* char *meas_tab[12][16]; */

int main(int argc, char *argv[]) {

    int i, j;
    
        
    for (int i=0; i<12; i++) {
        j = 0;
        while (meas_tab[i][j] != 0) {
            printf("%s ", meas_tab[i][j]);
            j++;
        }
        printf("\n");
    }

    char *sym;
    sym = strdup("uA");
    
    for (i=0; i<12; i++) {
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
    
    for (i=0; i<12; i++) {
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

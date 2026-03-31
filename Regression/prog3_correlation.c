/*
 * prog3_correlation.c
 * Computes Pearson r, r^2, direction, strength, and verifies r^2 = b*d.
 * Compile: gcc -o prog3 prog3_correlation.c -lm
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define MAX_ROWS 1000
static const char* interpret(double r) {
    double a = fabs(r);
    if (a>=0.9) return "Very strong";
    if (a>=0.7) return "Strong";
    if (a>=0.5) return "Moderate";
    if (a>=0.3) return "Weak";
    return "Very weak / negligible";
}
int main(void) {
    FILE *fp = fopen("data.csv","r");
    if (!fp) { fprintf(stderr,"Cannot open data.csv\n"); return 1; }
    double x[MAX_ROWS], y[MAX_ROWS];
    int n=0; char line[256];
    fgets(line,sizeof(line),fp);
    while (fgets(line,sizeof(line),fp) && n<MAX_ROWS)
        if (sscanf(line,"%lf,%lf",&x[n],&y[n])==2) n++;
    fclose(fp);
    double sum_x=0,sum_y=0,sum_xy=0,sum_x2=0,sum_y2=0;
    for (int i=0;i<n;i++) {
        sum_x+=x[i]; sum_y+=y[i]; sum_xy+=x[i]*y[i];
        sum_x2+=x[i]*x[i]; sum_y2+=y[i]*y[i];
    }
    double num   = n*sum_xy - sum_x*sum_y;
    double denom = sqrt((n*sum_x2-sum_x*sum_x)*(n*sum_y2-sum_y*sum_y));
    if (fabs(denom)<1e-12) { printf("Correlation undefined.\n"); return 1; }
    double r = num/denom, r2 = r*r;
    double b = (n*sum_xy-sum_x*sum_y)/(n*sum_x2-sum_x*sum_x);
    double d = (n*sum_xy-sum_x*sum_y)/(n*sum_y2-sum_y*sum_y);
    printf("=== Pearson Correlation Analysis ===\n");
    printf("n                 : %d\n", n);
    printf("r                 : %.6f\n", r);
    printf("r^2               : %.6f  (%.2f%% variance explained)\n", r2, r2*100.0);
    printf("Direction         : %s\n", r>0?"Positive":(r<0?"Negative":"None"));
    printf("Strength          : %s\n", interpret(r));
    printf("\nVerification: b*d = %.6f  (should equal r^2 = %.6f)\n", b*d, r2);
    return 0;
}

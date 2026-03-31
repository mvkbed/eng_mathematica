/*
 * prog1_basic_regression.c
 * Reads (x,y) from data.csv, computes y_hat = a + b*x via Least Squares.
 * Prints: n, x_bar, y_bar, b, a, SSR, SST, R^2
 * Compile: gcc -o prog1 prog1_basic_regression.c -lm
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define MAX_ROWS 1000
int main(void) {
    FILE *fp = fopen("data.csv", "r");
    if (!fp) { fprintf(stderr, "Cannot open data.csv\n"); return 1; }
    double x[MAX_ROWS], y[MAX_ROWS];
    int n = 0;
    char line[256];
    fgets(line, sizeof(line), fp); /* skip header */
    while (fgets(line, sizeof(line), fp) && n < MAX_ROWS)
        if (sscanf(line, "%lf,%lf", &x[n], &y[n]) == 2) n++;
    fclose(fp);
    if (n < 2) { fprintf(stderr, "Need at least 2 data points.\n"); return 1; }
    double sum_x=0, sum_y=0, sum_xy=0, sum_x2=0;
    for (int i=0; i<n; i++) {
        sum_x  += x[i]; sum_y  += y[i];
        sum_xy += x[i]*y[i]; sum_x2 += x[i]*x[i];
    }
    double x_bar = sum_x/n, y_bar = sum_y/n;
    double denom = n*sum_x2 - sum_x*sum_x;
    if (fabs(denom) < 1e-12) { fprintf(stderr,"All x identical; regression undefined.\n"); return 1; }
    double b = (n*sum_xy - sum_x*sum_y) / denom;
    double a = y_bar - b*x_bar;
    double SSR=0, SST=0;
    for (int i=0; i<n; i++) {
        double yh = a + b*x[i];
        SSR += (y[i]-yh)*(y[i]-yh);
        SST += (y[i]-y_bar)*(y[i]-y_bar);
    }
    double R2 = (SST > 1e-12) ? 1.0 - SSR/SST : 1.0;
    printf("=== Simple Linear Regression: y = a + b*x ===\n");
    printf("n         : %d\n", n);
    printf("x_bar     : %.6f\n", x_bar);
    printf("y_bar     : %.6f\n", y_bar);
    printf("Sum x     : %.6f\n", sum_x);
    printf("Sum y     : %.6f\n", sum_y);
    printf("Sum xy    : %.6f\n", sum_xy);
    printf("Sum x^2   : %.6f\n", sum_x2);
    printf("-------------------------------------------\n");
    printf("Slope  b  : %.6f\n", b);
    printf("Intercept a: %.6f\n", a);
    printf("Line      : y_hat = %.4f + %.4f * x\n", a, b);
    printf("-------------------------------------------\n");
    printf("SSR       : %.6f\n", SSR);
    printf("SST       : %.6f\n", SST);
    printf("R^2       : %.6f  (%.2f%% variance explained)\n", R2, R2*100.0);
    return 0;
}

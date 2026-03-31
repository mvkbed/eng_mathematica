/*
 * prog2_prediction.c
 * Fits regression from data.csv, then interactively predicts y from user-input x.
 * Compile: gcc -o prog2 prog2_prediction.c -lm
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define MAX_ROWS 1000
int main(void) {
    FILE *fp = fopen("data.csv", "r");
    if (!fp) { fprintf(stderr,"Cannot open data.csv\n"); return 1; }
    double x[MAX_ROWS], y[MAX_ROWS];
    int n = 0; char line[256];
    fgets(line, sizeof(line), fp);
    while (fgets(line, sizeof(line), fp) && n < MAX_ROWS)
        if (sscanf(line, "%lf,%lf", &x[n], &y[n]) == 2) n++;
    fclose(fp);
    double sum_x=0,sum_y=0,sum_xy=0,sum_x2=0;
    for (int i=0;i<n;i++) { sum_x+=x[i]; sum_y+=y[i]; sum_xy+=x[i]*y[i]; sum_x2+=x[i]*x[i]; }
    double x_bar=sum_x/n, y_bar=sum_y/n;
    double b = (n*sum_xy - sum_x*sum_y)/(n*sum_x2 - sum_x*sum_x);
    double a = y_bar - b*x_bar;
    printf("Regression fitted: y_hat = %.4f + %.4f * x\n", a, b);
    printf("Data x-range: [%.2f, %.2f]\n\n", x[0], x[n-1]);
    char again = 'y';
    while (again=='y'||again=='Y') {
        double xval;
        printf("Enter x to predict y: ");
        if (scanf("%lf",&xval)!=1) break;
        printf("  --> y_hat = %.6f\n\n", a + b*xval);
        printf("Predict another? (y/n): ");
        scanf(" %c",&again);
    }
    printf("Done.\n");
    return 0;
}

/*
 * prog4_two_lines.c
 * Computes both regression lines:
 *   y on x: y_hat = a + b*x
 *   x on y: x_hat = c + d*y
 * Verifies intersection at (x_bar, y_bar). User chooses which to predict from.
 * Compile: gcc -o prog4 prog4_two_lines.c -lm
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define MAX_ROWS 1000
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
    double N=(double)n, x_bar=sum_x/N, y_bar=sum_y/N;
    double b = (N*sum_xy-sum_x*sum_y)/(N*sum_x2-sum_x*sum_x);
    double a = y_bar - b*x_bar;
    double d = (N*sum_xy-sum_x*sum_y)/(N*sum_y2-sum_y*sum_y);
    double c = x_bar - d*y_bar;
    double r = (b>0?1:-1)*sqrt(fabs(b*d));
    printf("=== Two Lines of Regression ===\n");
    printf("x_bar = %.6f,  y_bar = %.6f\n\n", x_bar, y_bar);
    printf("Line 1 (y on x): y_hat = %.6f + %.6f * x\n", a, b);
    printf("Line 2 (x on y): x_hat = %.6f + %.6f * y\n\n", c, d);
    printf("r = sqrt(b*d) = %.6f,  r^2 = %.6f\n\n", r, b*d);
    double bd=b*d;
    if (fabs(1-bd)>1e-12) {
        double xi=(c+d*a)/(1-bd), yi=a+b*xi;
        printf("Intersection: (%.6f, %.6f)  [expected: (%.6f, %.6f)]\n\n",xi,yi,x_bar,y_bar);
    }
    int choice;
    printf("Predict using:\n  1 -> y from x\n  2 -> x from y\nChoice: ");
    scanf("%d",&choice);
    if (choice==1) {
        double xv; printf("Enter x: "); scanf("%lf",&xv);
        printf("y_hat = %.6f\n", a+b*xv);
    } else if (choice==2) {
        double yv; printf("Enter y: "); scanf("%lf",&yv);
        printf("x_hat = %.6f\n", c+d*yv);
    } else printf("Invalid choice.\n");
    return 0;
}

/*
 * prog5_residuals.c
 * Prints full residuals table: i, x_i, y_i, y_hat_i, e_i, e_i^2, outlier flag.
 * Outlier: |e_i| > 2*s where s = sqrt(SSR/(n-2))
 * Compile: gcc -o prog5 prog5_residuals.c -lm
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
    double sum_x=0,sum_y=0,sum_xy=0,sum_x2=0;
    for (int i=0;i<n;i++) { sum_x+=x[i]; sum_y+=y[i]; sum_xy+=x[i]*y[i]; sum_x2+=x[i]*x[i]; }
    double x_bar=sum_x/n, y_bar=sum_y/n;
    double b=(n*sum_xy-sum_x*sum_y)/(n*sum_x2-sum_x*sum_x);
    double a=y_bar-b*x_bar;
    double e[MAX_ROWS], SSR=0, sum_e=0, SST=0;
    for (int i=0;i<n;i++) {
        e[i]=y[i]-(a+b*x[i]); SSR+=e[i]*e[i]; sum_e+=e[i];
        SST+=(y[i]-y_bar)*(y[i]-y_bar);
    }
    double s=sqrt(SSR/(n-2));
    printf("Regression: y_hat = %.4f + %.4f * x\n\n",a,b);
    printf("%-4s  %-8s  %-8s  %-10s  %-12s  %-10s  %s\n",
           "i","x_i","y_i","y_hat_i","e_i","e_i^2","Outlier?");
    printf("%-4s  %-8s  %-8s  %-10s  %-12s  %-10s  %s\n",
           "----","--------","--------","----------","------------","----------","--------");
    for (int i=0;i<n;i++) {
        double yh=a+b*x[i];
        printf("%-4d  %-8.4f  %-8.4f  %-10.4f  %-12.6f  %-10.6f  %s\n",
               i+1,x[i],y[i],yh,e[i],e[i]*e[i],fabs(e[i])>2*s?"YES *":"no");
    }
    printf("\nSum of residuals (should be ~0): %.2e\n",sum_e);
    printf("SSR = %.6f\nSST = %.6f\nR^2 = %.6f\n",SSR,SST,1.0-SSR/SST);
    printf("s (residual std dev) = %.6f,  2s = %.6f\n",s,2*s);
    return 0;
}

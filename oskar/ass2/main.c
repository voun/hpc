# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <complex.h>
# include <math.h>
# include <unistd.h>

double complex *roots;



double complex mypow(double complex z, int d);
double complex newton(double complex z0, int d);
double complex update(double complex z, int d);
int convergence(double complex z, int d);


int main(int argc, char *argv[]){
    if (argc != 4) {
        printf("Need 3 input arguments. Exiting!\n");
        exit(1);
    }

    char l_str[10], t_str[10];
    int d, l, t;
    for (size_t i = 1; i<argc; i++) {
        if (argv[i][0] == '-') {
            if (argv[i][1] == 't'){
                strncpy(t_str, argv[i]+2, strlen(argv[i])-2);
                t = atoi(t_str);
            } else if (argv[i][1] == 'l') {
                strncpy(l_str, argv[i]+2, strlen(argv[i])-2);
                l = atoi(l_str);
            }
        } else {
            d = strtol(argv[i], NULL, 0);
        }
    }


    printf("d = %d \nl = %d \nt = %d\n", d, l, t);
    roots = malloc(sizeof(double complex)*d);
    double arg;
    for (size_t i = 0; i<d; i++){
        arg = 2*M_PI*i / d;
        roots[i] = cos(arg) + I*sin(arg);
        printf("root = %f + i%f\n", creal(roots[i]), cimag(roots[i]));
    }


    double complex z1 = 1.0 + 3.0 * I;
    double complex res;
    res = newton(z1, d);
    printf("\n%f + i%f\n", creal(res), cimag(res));


    return 0;

}

double complex newton(double complex z0, int d){


    double complex res;
    double complex z = z0;
    while (convergence(z, d) == 0) {
       // printf("z = %f + i%f\n", creal(z), cimag(z));
        //sleep(1);
        z = update(z,d);
    }
    return z;

}

int convergence(double complex z, int d) {
   // double llim = 1-1/(10*10*10);
   // double ulim = 1+1/(10*10*10);
   // if (cabs(z) >= ulim || cabs(z) <= llim )
   //     return 0;


    for (size_t i = 0; i < d; i++) {
        double val = cabs(z - roots[i]);
        //printf("abs = %f \n", val);
        if (val <= 0.001)
            return 1;
    }
    return 0;
}



double complex update(double complex z, int d) {
    double complex res;
    double denom = cabs(z)*cabs(z);
    res = (z*(d-1) + mypow( (creal(z)-cimag(z))/denom, d-1)) /d;

   // double arg = carg(z);
   // double real = creal(z)*(d-1) + cos((d-1)*arg);
   // double imag = cimag(z)*(d-1) + sin((d-1)*arg);
   // res = (real + imag)/d;

    return res;
}


double complex mypow(double complex z, int d) {
    double complex res;
    switch(d) {
        case 0: res = 1; break;
        case 1: res = z; break;
        case 2: res = z*z; break;
        case 3: res = z*z*z; break;
        case 4: res = z*z*z*z; break;
        case 5: res = z*z*z*z*z; break;
        case 6: res = z*z*z*z*z*z; break;
        case 7: res = z*z*z*z*z*z*z; break;
        case 8: res = z*z*z*z*z*z*z*z; break;
        case 9: res = z*z*z*z*z*z*z*z*z; break;
        case 10: res = z*z*z*z*z*z*z*z*z*z; break;
        default:
            fprintf(stderr, "Unexpected degree: %d\n", d);
            exit(1);
    }
    return res;

}





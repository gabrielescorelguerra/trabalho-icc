#include <stdio.h>
#include <stdlib.h>
#include <likwid.h>

// #include "utils.h"
#include "NewtonSNLB.h"

/* 
n: dimensão do SNLB
x0: valor inicial das variáveis
epsilon: tolerância a ser considerada nos critérios de parada do Método de Newton
MAXIT: número máximo de iterações do Método de Newton
*/

int main() {
    int n, MAXIT;
    double x0, epsilon;

    // le entradas
    scanf("%d", &n);
    scanf("%lf", &x0);
    scanf("%lf", &epsilon);
    scanf("%d", &MAXIT);

    // inicializa o LIKWID
    LIKWID_MARKER_INIT;

    // aloca memória para o vetor de variáveis, inicializando todas com x0
    double *X = (double *) malloc(sizeof(double) * n);
    for (int i = 0; i < n; ++i) {
        X[i] = x0;
    }

    // executa o método de Newton para resolver o sistema não linear de Broyden
    LIKWID_MARKER_START("newton");
    newton(X, epsilon, n, MAXIT);
    LIKWID_MARKER_STOP("newton");

    // finaliza o LIKWID
    LIKWID_MARKER_CLOSE;

    // libera a memória alocada para o vetor de variáveis
    free(X);

    return 0;
}


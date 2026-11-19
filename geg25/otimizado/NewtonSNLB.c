#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <likwid.h>

#include "SistNaoLinearBroyden.h"
#include "NewtonSNLB.h"
#include "EliminacaoGauss.h"
#include "utils.h"

int newton(double *X, double epsilon, int n, int MAXIT) {

    LIKWID_MARKER_START("newton");

    // Temporizadores
    double tempoTotal = timestamp();
    double tempoJ = 0.0;
    double tempo_sisLinear = 0.0;

    // Aloca memória para F, J e S
    double *F = (double *) malloc(sizeof(double) * n);
    double *S = (double *) malloc(sizeof(double) * n); 
    double *Jdiag = (double *) malloc(sizeof(double) * n);

    // imprime o vetor de variáveis X inicial
    for (int i = 0; i < n; ++i) {
        printf("x%d = %lf\n", i + 1, X[i]);
    }
    printf("#");

    // loop de iteracoes
    for (int iter = 0; iter < MAXIT; ++iter) {
        // Calcula F(X) e J(X) para a iteracao atual
        calcF(F, X, n);

        double tempoInicJ = timestamp();
        LIKWID_MARKER_START("calcJ");
        calcJ(Jdiag, X, n);
        LIKWID_MARKER_STOP("calcJ");
        tempoJ += timestamp() - tempoInicJ;

        // inverte o sinal de F para resolver o sistema linear J * deltaX = -F
        for (int i = 0; i < n; ++i) {
            F[i] = -F[i];
        }

        // resolve o sistema linear J * deltaX = -F para encontrar deltaX por eliminacao de Gauss
        double tempoInicSisLinear = timestamp();
        LIKWID_MARKER_START("eliminacaoGauss");
        eliminacaoGauss(Jdiag, S, F, n);
        LIKWID_MARKER_STOP("eliminacaoGauss");
        tempo_sisLinear += timestamp() - tempoInicSisLinear;

        // Atualiza X com as soluções aproximadas.
        for (int i = 0; i < n; ++i) {
            X[i] += S[i];
        }

        // imprime o vetor de variáveis X atual
        for (int i = 0; i < n; ++i) {
            printf("\nx%d = %lf", i + 1, X[i]);
        }
        printf("\n#");

        // Verifica os critérios de parada com base em epsilon. CONFERIR
        if (norma(S, n) < epsilon) {
            break;
        }
    }

    // imprime tempos
    tempoTotal = timestamp() - tempoTotal;

    printf("##########\n%.15g\n%.15g\n%.15g\n###########\n", tempoTotal, tempoJ, tempo_sisLinear);

    // Libera a memória alocada para F, J e S
    free(F);
    free(Jdiag);
    free(S);

    LIKWID_MARKER_STOP("newton");
  
    return 0; // Retorna 0 para indicar sucesso (ou outro valor para indicar falha)
}

// define o calculo da norma do vetor V de tamanho n
// Aqui: norma infinito
double norma(double *V, int n) {
  double max = fabs(V[0]);
  for (int i = 1; i < n; ++i)
    if (fabs(V[i]) > max)
      max = fabs(V[i]);
  return max;
}
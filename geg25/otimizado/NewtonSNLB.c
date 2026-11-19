#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <likwid.h>

#include "SistNaoLinearBroyden.h"
#include "NewtonSNLB.h"
#include "EliminacaoGauss.h"
#include "utils.h"

int newton(double *X, double epsilon, int n, int MAXIT) {
    // Temporizadores
    double tempoJ = 0.0;
    double tempo_sisLinear = 0.0;

    // vai armazenar as saidas das iteracoes
    double *valoresX = (double *) malloc(sizeof(double) * (MAXIT + 1) * n);
    int iteracoes = 0; // quantas iteracoes foram guardadas alem da inicial

    // guarda o vetor de variáveis X inicial (iteracao 0)
    for (int i = 0; i < n; ++i) {
        valoresX[i] = X[i];
    }

    // a contagem de tempo comeca aqui: a alocacao de F, J e S entra, a de valoresX nao
    double tempoTotal = timestamp();
    LIKWID_MARKER_START("newton");

    // Aloca memória para F, J e S
    double *F = (double *) malloc(sizeof(double) * n);
    double *S = (double *) malloc(sizeof(double) * n);
    double *Jdiag = (double *) malloc(sizeof(double) * n);

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

        // guarda o vetor de variáveis X atual
        iteracoes++;
        for (int i = 0; i < n; ++i) {
            valoresX[iteracoes * n + i] = X[i];
        }

        // Verifica os critérios de parada com base em epsilon. CONFERIR
        if (norma(S, n) < epsilon) {
            break;
        }
    }

    LIKWID_MARKER_STOP("newton");

    tempoTotal = timestamp() - tempoTotal;

    // dai acaba aqui 

    // imprime todas as iteracoes (a 0 eh o X inicial), separadas por '#'
    for (int k = 0; k <= iteracoes; ++k) {
        for (int i = 0; i < n; ++i) {
            printf("x%d = %lf\n", i + 1, valoresX[k * n + i]);
        }
        if (k < iteracoes) {
            printf("#\n");
        }
    }

    // imprime tempos
    printf("###########\n%.15g\n%.15g\n%.15g\n###########\n", tempoTotal, tempoJ, tempo_sisLinear);

    // Libera a memória alocada para F, J e S
    free(F);
    free(Jdiag);
    free(S);
    free(valoresX);
  
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
#include <math.h>
#include "EliminacaoGauss.h"

// eliminação de Gauss no sistema linear SL -> solução em X
// AX = b, n eh a dimensão do sistema
void eliminacaoGauss(double **A, double *X, double *b, int n) {
  triang(A, b, n);
  retrossubs(A, X, b, n);
}

// triangularização
void triang(double **A, double *b, int n) {
    for (int i = 0; i < n - 1; ++i) {

        // pivot(A, b, i, n);

        double m = A[i+1][i] / A[i][i];

        A[i+1][i] = 0;
        A[i+1][i+1] -= -2 * m;
        b[i+1] -= b[i] * m;
    }
}

// retrossubstituição
void retrossubs(double **A, double *X, double *b, int n) {
    // resolve o sistema triangular superior AX = b
    // porque so tem ele e o do lado, dai multiplicando fica assim 

    // ultima linha
    X[n-1] = b[n-1] / A[n-1][n-1];

    for (int i = n-2; i >=0; --i) {
        X[i] = (b[i] + 2*X[i + 1]) / A[i][i]; 
    }
}

// Efetua pivoteamento parcial na linha i
void pivot(double **A, double *b, int i, int n) {
    // encontra o maior valor absoluto na coluna i a partir da linha i
    double max = fabs(A[i][i]);
    int max_i = i;

    for (int j = i+1; j < n; ++j) {
        double v = fabs(A[j][i]);
        if (v > max) {
            max = v;
            max_i = j;
        }
    }

    // se o maior valor não está na linha i, troca as linhas
    if (max_i != i) {
        for (int j = 0; j < n; ++j) {
            double tmp = A[i][j];
            A[i][j] = A[max_i][j];
            A[max_i][j] = tmp;
        }
        double tmp = b[i];
        b[i] = b[max_i];
        b[max_i] = tmp;
    }
}


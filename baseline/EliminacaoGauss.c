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
    for (int i = 0; i < n; ++i) {
    pivot(A, b, i, n);
    
    // elimina as entradas abaixo do pivô
    for (int k = i+1; k < n; ++k) {
        double m = A[k][i] / A[i][i];
        A[k][i] = 0.0;
        for (int j = i+1; j < n; ++j)
            A[k][j] -= A[i][j] * m;
        b[k] -= b[i] * m;
    }
  }
}

// retrossubstituição
void retrossubs(double **A, double *X, double *b, int n) {
    // resolve o sistema triangular superior AX = b
    for (int i = n-1; i >=0; --i) {
        X[i] = b[i];
        for (int j = i+1; j < n; j++)
            X[i] -= A[i][j] * X[j];
        X[i] /= A[i][i];
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


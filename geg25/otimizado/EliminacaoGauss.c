#include <math.h>
#include "EliminacaoGauss.h"

// eliminação de Gauss no sistema linear SL -> solução em S
// JdiagS = b, n eh a dimensão do sistema

// DEPOIS MUDAR NOME

void eliminacaoGauss(double *Jdiag, double *S, double *f, int n) {
  triang(Jdiag, f, n);
  retrossubs(Jdiag, S, f, n);
}

// triangularização
void triang(double *Jdiag, double *f, int n) {
    for (int i = 0; i < n - 1; ++i) {
        double m = -1 / Jdiag[i];

        // Jdiag[i+1][i] = 0;  isso eh visto como consequencia no calculo

        Jdiag[i+1] -= -2 * m;

        f[i+1] -= f[i] * m;
    }
}

// retrossubstituição
void retrossubs(double *Jdiag, double *S, double *f, int n) {
    // resolve o sistema triangular superior JdiagS = b
    // porque so tem ele e o do lado, dai multiplicando fica assim 

    // ultima linha
    S[n-1] = f[n-1] / Jdiag[n-1];

    for (int i = n-2; i >=0; --i) {
        S[i] = (f[i] + 2*S[i + 1]) / Jdiag[i]; 
    }
}

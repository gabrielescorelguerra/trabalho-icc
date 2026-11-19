#ifndef __SIST_NAO_LINEAR__
#define __SIST_NAO_LINEAR__

// calcula o valor de cada função do SNLB de dimensao n, com o vetor de variáveis X -> resultado em F
void calcF(double *F, double *X, int n);

// calcula o valor da matriz Jacobiana com o vetor de variáveis X -> resultado em JX
void calcJ(double *Jdiag, double *X, int n);

#endif
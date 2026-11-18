/*
    Método de Newton para a resolução de Sistemas Não Lineares nxn

    prof. Guilherme A. Derenievicz (DINF/ET/UFPR)

    Entrada:
    <n>
    <exp f1(x)>
    <exp f2(x)>
    ...
    <exp fn(x)>
    <aproximação inicial x1>
    <aproximação inicial x2>
    ...
    <aproximação inicial xn>
    <epsilon>
    <max iterações>

    Obs: variáveis devem ser x1, x2, ..., xn.

    TODO: desalocar memória
*/

#include <stdio.h>
#include <stdlib.h>
#include "SistNaoLinear.h"
#include "NewtonSNL.h"
#include "utils.h"

int main() {

/* 
N: dimensão do SNLB
x0: valor inicial das variáveis (um único, igual para todas as variáveis)
epsilon: tolerância a ser considerada nos critérios de parada do Método de Newton
MAX: número máximo de iterações do Método de Newton
*/

  unsigned int n;
  double epsilon, *X, Ttotal, Tgauss, Tjacobiana, Tderivadas;
  int MAXIT;
  char **f;

  while (scanf("%d",&n) != EOF) {

    // aloca memória para o vetor de funções
    f = (char **) malloc(sizeof(char *)*n);
    for (int i = 0; i < n; ++i) {
      f[i] = (char *) malloc(sizeof(char)*MAXLEN_FUNCS);
      scanf("%s", f[i]);
    }

    // aloca memória para o vetor de variáveis
    X = (double *) malloc(sizeof(double)*n);

    // le aproximações iniciais
    for (int i = 0; i < n; ++i)
      scanf("%lf", &(X[i]));

      // le tolerância e número máximo de iterações
    scanf("%lf %d", &epsilon, &MAXIT);

    //  cria o sistema não linear
    SistNaoLinear_t *SNL = criaSistNaoLinear(f, n, &Tderivadas);
    printSistNaoLinear(SNL);

    newton(SNL, X, epsilon, epsilon, MAXIT, &Ttotal, &Tgauss, &Tjacobiana);

    printf("###########\n");
    printf("# Tempo Total: %lf ms\n", Ttotal);
    printf("# Tempo Derivadas: %lf ms\n", Tderivadas);
    printf("# Tempo Jacobiana: %lf ms\n", Tjacobiana);
    printf("# Tempo SL: %lf ms\n", Tgauss);
    printf("###########\n\n");

    destroiSistNaoLinear(SNL);
    free(X);
    for (int i = 0; i < n; ++i)
      free(f[i]);
    free(f);
  }

  return 0;
}


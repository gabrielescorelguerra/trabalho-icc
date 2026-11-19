// calcula o valor de cada função do SNLB de dimensao n, com o vetor de variáveis X -> resultado em F
void calcF(double *F, double *X, int n) {
    F[0] = (-2 * X[0] * X[0]) + (3 * X[0]) + (-2 * X[1]) + 1;

    for (int i = 1; i <= n - 2; i++) {
        F[i] = (-2 * X[i] * X[i]) + (3 * X[i]) - X[i - 1] - (2 * X[i + 1]) + 1;
    }

    F[n - 1] = (-2 * X[n - 1] * X[n - 1]) + (3 * X[n - 1]) - X[n - 2];
}


// calcula o valor da matriz Jacobiana com o vetor de variáveis X -> resultado em JX
void calcJ(double **J, double *X, int n) {
    // calcula as derivadas parciais de cada funcão do SNL com o vetor de variáveis X
    J[0][0] = (-4 * X[0]) + 3;
    J[0][1] = -2;

    for (int i = 1; i <= n - 2; i++) {
        J[i][i - 1] = -1;
        J[i][i] = (-4 * X[i]) + 3;
        J[i][i + 1] = -2;
    }

    J[n - 1][n - 2] = -1;
    J[n - 1][n - 1] = (-4 * X[n - 1]) + 3;
}
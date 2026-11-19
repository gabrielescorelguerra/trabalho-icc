// eliminação de Gauss no sistema linear SL -> solução em X
// AX = b, n eh a dimensão do sistema
void eliminacaoGauss(double **A, double *X, double *b, int n);

void triang(double **A, double *b, int n);

// retrossubstituição
void retrossubs(double **A, double *X, double *b, int n);

void pivot(double **A, double *b, int i, int n);


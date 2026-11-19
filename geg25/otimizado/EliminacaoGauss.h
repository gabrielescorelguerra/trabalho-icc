// eliminação de Gauss no sistema linear SL -> solução em X
// AX = b, n eh a dimensão do sistema
void eliminacaoGauss(double *Jdiag, double *S, double *f, int n);

void triang(double *Jdiag, double *f, int n);

// retrossubstituição
void retrossubs(double *Jdiag, double *S, double *f, int n);

void pivot(double *Jdiag, double *f, int i, int n);


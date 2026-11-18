A. Código-fonte em C

0/10 concluídos
[ ] Implementação do método de Newton para sistemas não lineares
[ ] Implementação das funções de Broyden
[ ] Implementação do cálculo da matriz Jacobiana
[ ] Implementação da resolução dos sistemas lineares
[ ] Representação de matrizes e vetores
[ ] Programa principal em módulo separado, contendo apenas main()
[ ] Funções organizadas em módulos específicos
[ ] Arquivos .h e .c organizados corretamente
[ ] Comentários adequados descrevendo cada função
[ ] Tratamento de alocação e liberação de memória

B. Compilação e execução

0/11 concluídos
[ ] Makefile com regra all
[ ] Makefile com regra clean
[ ] Compilação dos módulos individualmente
[ ] Executável gerado com o nome broyden
[ ] Compilação com GCC e flags -O3 -march=native -mavx -fopt-info-vec
[ ] Programa executável através de ./broyden
[ ] Leitura de N, x0, epsilon e MAX pela entrada padrão
[ ] Saída no formato exigido pelo enunciado
[ ] Impressão de todas as iterações separadas por #
[ ] Impressão dos três tempos em milissegundos
[ ] Execução correta sem falhas de segmentação

C. Instrumentação e desempenho

0/12 concluídos
[ ] Marcador LIKWID para execução total do Newton
[ ] Marcador LIKWID para cálculo das Jacobianas
[ ] Marcador LIKWID para resolução dos sistemas lineares
[ ] Medição dos tempos com timestamp() da biblioteca utils
[ ] Medições com grupo MEM ou L3
[ ] Medições com grupo L2CACHE ou alternativa permitida
[ ] Medições com grupo FLOPS_DP e FLOPS_AVX
[ ] Testes nas 12 dimensões exigidas: 32 até 20000
[ ] MAX = 25 e epsilon = 0 em todos os benchmarks
[ ] Comparação entre versão sem e com otimizações
[ ] Testes realizados na mesma máquina
[ ] Identificação da arquitetura do processador com likwid-topology -g -c

D. Relatório científico em PDF

0/15 concluídos
[ ] Texto no formato de artigo científico
[ ] Descrição do problema e do método de Newton
[ ] Descrição da implementação e das estruturas de dados
[ ] Justificativa dos métodos escolhidos para os sistemas lineares
[ ] Descrição das otimizações implementadas e onde foram aplicadas
[ ] Justificativa técnica de cada otimização
[ ] Metodologia e configuração experimental
[ ] Identificação da arquitetura do processador
[ ] 12 gráficos de desempenho
[ ] Eixos de N em escala logarítmica
[ ] Eixos de tempo em escala logarítmica nos gráficos de tempo
[ ] Gráficos com linhas distintas para as versões comparadas
[ ] Explicação e interpretação de cada gráfico
[ ] Conclusões sobre o desempenho observado
[ ] Declaração do uso de IA, se utilizada

E. Empacotamento e envio

0/8 concluídos
[ ] Pasta principal com o login correto
[ ] Makefile e todos os fontes incluídos
[ ] Scripts auxiliares incluídos
[ ] PDF com nome correto
[ ] Arquivo compactado em ZIP ou TAR
[ ] Testar a compilação a partir do pacote descompactado
[ ] Testar o executável a partir do pacote descompactado
[ ] Enviar pelo Moodle antes de 19/11/2026 às 23h59
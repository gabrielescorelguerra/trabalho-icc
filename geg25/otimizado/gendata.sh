#!/bin/bash

# descobre a pasta onde o script ta
# $0 é o caminho usado para rodar o script
# dirname pega so o nome do diretorio desse caminho
CMD_DIR=`dirname $0`

PROG=${1:-broyden} # define para usar o 1o argumento e, caso nao exista, usar matmul


CPU=${2:-3} # determina o core usado pelo likwid



# define onde os dados serao armazenados
DATA_DIR="resultados"
TEMPOS="${DATA_DIR}/Tempos.csv"

# metricas a serem avaliadas pelo likwid
METRICA="MEM L2CACHE FLOPS_DP FLOPS_AVX"

# tamanhos
TAMANHOS="32 64 128 256 512 1000 2000 4000" # 8000 9000 10000 20000"

X0=-1
EPS=0
MAXIT=25

# cria, se nao houver, o diretorio de saida
# -p cria os intermediarios tambem
mkdir -p ${DATA_DIR}

# define frequencia de CPU
likwid-setFrequencies -g performance -c ${CPU}

# remove os arquivos gerados na ultima compilacao, CREIO QUE SERIA O CLEAN DAI? VER OQ FAZER COM ISSO
# make purge
# compila novamente por meio do makefile
make

# remove o csv de tempos de execucoes anteriores
rm -f ${TEMPOS}

# pra cada metrica (mem, flops, cache)
for m in ${METRICA}
do
    # define o arquivo cada metrica, no caso serao 4, um pra cada area
    LIKWID_LOG="${DATA_DIR}/${m}.log"
    # remove resultados anteriores
    rm -f ${LIKWID_LOG}
    
    # pra cada tamanho a ser testado
    for n in $TAMANHOS
    do
        # cria um arquivo temporario que vai receber os resultados da execucao especifica
        # exemplo: resultados/FLOPS_DP_128.txt
        LIKWID_OUT="${DATA_DIR}/${m}_${n}.txt"
        
        # impreme o que esta sendo executado
        # --->> metria ./programa tamanho
        # so para conseguir acompanhar o que esta sendo executado, nao tem efeito na saida do programa
        echo "n: ${n} , x0: ${X0}, EPS: ${EPS}, MAXIT: ${MAXIT} --->>  $m: ./${PROG}" >/dev/tty

        # roda o comando do perfctr e a saida vai para LIKWID_OUT
        # -C : cores
        # -g : grupo atual, metrica
        # -o : arquivo que recebe a saida
        # -m : ativa o modo marker api
        # ./PROG -n n : executa o programa pra entrada n
        # >> ${TEMPOS} : redirecionamento do bash, diz para acresentar isso no arquivo de tempos
        printf "%s\n%s\n%s\n%s\n" "$n" "$X0" "$EPS" "$MAXIT" | likwid-perfctr -O -C ${CPU} -g ${m} -o ${LIKWID_OUT} -m ./${PROG} \
                | awk '/^###########$/ {f = !f; next} f' | paste -sd, - | sed "s/^/${n},/" >> ${TEMPOS}
            
        # echo "===> N: ${n} <==" >> ${LIKWID_LOG}
        # pega o conteudo do temporario e junta no log, que eh referente a metrica
        cat ${LIKWID_OUT} >> ${LIKWID_LOG}
        # apaga o temporario
        rm -f ${LIKWID_OUT}
    done

    # transforma o python num csv e passa para genplot gerar grafico (atividade da proxima aula)
   # ${CMD_DIR}/genplot.py < ${LIKWID_LOG} > ${LIKWID_LOG%%.log}.csv
done

# restaura o estado apos o experimento
likwid-setFrequencies -g powersave -c ${CPU}

echo ""
echo ""

#! /usr/bin/env python3

# Descomente as 2 linhas abaixo se quiser que Python procure por modulos
# em outros diretórios que não os do sistema e o diretório corrente.
## import sys, os
## sys.path.extend([ '~/lib/python', '.', '..' ])
# Ou em seu ambiente shell, crie e exporte a variavel de ambiente PYTHONPATH.
# Coloque a linha abaixo em seu arquivo '~/.bashrc' ou '~/.profile:
## export PYTHONPATH="${HOME}/lib/python:.:.."
#
# import numpy as np
# import matplotlib.pyplot as plt
import os
import re

import matplotlib

matplotlib.use("Agg")  # so salva as imagens, nao tenta abrir janela
import matplotlib.pyplot as plt

#  "CACHE" : "data cache misses",
#  "CACHE" : "data cache miss ratio",
#  "CACHE" : "data cache miss rate",
#  "L2CACHE" : "L2 miss ratio",
#  "L2CACHE" : "L2 miss rate",
#  "L3" : "L3 bandwidth \[*MBytes/s\]*",
#  "MEM" : "Memory bandwidth \[*MBytes/s\]*",
#  "MEM" : "Memory read bandwidth",
#  "MEM" : "Memory write bandwidth",
#  "TIME" : "Runtime \(RDTSC\) \[*s\]*",
#  "ENERGY" : "Power \[*W\]*"


# nome dos campos que serao lidos dos arquivos de saida do likwid-perfctr -O
campos = {
    "L2CACHE": "L2 miss ratio",
    "FLOPS_DP": r"DP \[*MFLOP/s\]*",
    "FLOPS_AVX": r"Packed DP \[*MFLOP/s\]*",
    "MEM": r"Memory bandwidth \[*MBytes/s\]*",
}

# testes do likwid, um grafi-co por trecho cada.
# nome: (texto do eixo y, [(rotulo da linha, grupo do likwid)])
# o teste de FLOPS junta os grupos FLOPS_DP e FLOPS_AVX no mesmo grafico
testes = {
    "MEM": ("Memory bandwidth [MBytes/s]", [("", "MEM")]),
    "L2CACHE": ("L2 miss ratio", [("", "L2CACHE")]),
    "FLOPS": ("MFLOP/s", [("DP", "FLOPS_DP"), ("AVX", "FLOPS_AVX")]),
}

# cor de cada versao; estilo de linha de cada grupo dentro do mesmo grafico
cores = {"base": "tab:blue", "otim": "tab:red"}
estilos = ("-", "--")

# nomes dos marcadores likwid usados no codigo C
trechos = ("newton", "calcJ", "eliminacaoGauss")

DIR_BASE = "../base/resultados"
DIR_OTIM = "./resultados"

# tamanhos na MESMA ordem em que foram rodados no gendata.sh (TAMANHOS)
tamanhos_base = (32, 64, 128)
tamanhos_otim = (32, 64, 128, 256)


# 'STRUCT,Info' inicia Região
# 'TABLE,Region.*Metric,<metrica>' inicia metrica
# '<string>' inicia linha '<string>.*,<valor>,.*', onde
# <string> é obtido a partr de 'campo[]', tendo <metrica> como chave


def lerDados(arquivo, trecho, campo, tamanhos):
    """Le o log de um grupo do likwid e devolve {N: valor} do campo no trecho.

    O log nao diz qual N gerou cada bloco, entao o i-esimo valor encontrado
    do trecho e associado ao i-esimo tamanho de 'tamanhos'.
    """
    valores = []
    dentro = False  # True enquanto estamos na tabela de metricas do trecho

    with open(arquivo) as f:
        for linha in f:
            if linha.startswith("TABLE"):
                # ex: "TABLE,Region calcJ,Group 1 Metric,MEM,10"
                partes = linha.split(",")
                dentro = partes[1] == "Region " + trecho and "Metric" in partes[2]
            elif dentro and re.match(campo, linha):
                # ex: "Memory bandwidth [MBytes/s],2633.2558,"
                valores.append(float(linha.split(",")[1]))
                dentro = False

    if len(valores) != len(tamanhos):
        print(
            f"AVISO: {arquivo} ({trecho}): {len(valores)} valores "
            f"para {len(tamanhos)} tamanhos"
        )

    return dict(zip(tamanhos, valores))


def lerTempos(arquivo):
    """Le o Tempos.csv (linhas 'N,total,jacobiana,sistema') e devolve
    {trecho: {N: tempo medio}}. Cada N aparece varias vezes (uma por grupo
    do likwid), entao tira a media.
    """
    colunas = {"newton": 1, "calcJ": 2, "eliminacaoGauss": 3}
    medidas = {trecho: {} for trecho in colunas}  # {trecho: {N: [tempos]}}

    with open(arquivo) as f:
        for linha in f:
            partes = linha.strip().split(",")
            if len(partes) != 4 or not partes[0].isdigit():
                continue  # pula cabecalho e linhas estranhas
            n = int(partes[0])
            for trecho, i in colunas.items():
                medidas[trecho].setdefault(n, []).append(float(partes[i]))

    return {
        trecho: {n: sum(v) / len(v) for n, v in porN.items()}
        for trecho, porN in medidas.items()
    }


def gera_grafico(series, ylabel, titulo, arquivo, ylog=False):
    """series: lista de (legenda, {N: valor}, cor, estilo de linha)."""
    fig, ax = plt.subplots()
    Ns = set()

    for legenda, dados, cor, estilo in series:
        ns = sorted(dados)
        ys = [dados[n] for n in ns]
        ax.plot(ns, ys, marker="o", color=cor, linestyle=estilo, label=legenda)
        Ns |= set(ns)

    Ns = sorted(Ns)
    ax.set_xscale("log")
    if ylog:
        ax.set_yscale("log")
    ax.set_xticks(Ns)
    ax.set_xticklabels(Ns)  # mostra os N reais, não 10^x
    ax.set_xlabel("N")
    ax.set_ylabel(ylabel)
    ax.set_title(titulo)
    ax.legend()
    ax.grid(True)

    os.makedirs("graficos", exist_ok=True)
    fig.savefig(f"graficos/{arquivo}.png", dpi=150)  # salva como imagem
    plt.close(fig)


def main():
    versoes = (
        ("base", DIR_BASE, tamanhos_base),
        ("otim", DIR_OTIM, tamanhos_otim),
    )

    for trecho in trechos:
        # teste de tempo (eixo y em log)
        series = []
        for nome, diretorio, _ in versoes:
            tempos = lerTempos(f"{diretorio}/Tempos.csv")[trecho]
            series.append((nome, tempos, cores[nome], "-"))
        gera_grafico(
            series, "Tempo medio [ms]", f"Tempo: {trecho}", f"tempo_{trecho}", ylog=True
        )

        # testes do likwid
        for teste, (ylabel, grupos) in testes.items():
            series = []
            for nome, diretorio, tamanhos in versoes:
                for (rotulo, grupo), estilo in zip(grupos, estilos):
                    dados = lerDados(
                        f"{diretorio}/{grupo}.log", trecho, campos[grupo], tamanhos
                    )
                    legenda = f"{nome} - {rotulo}" if rotulo else nome
                    series.append((legenda, dados, cores[nome], estilo))
            gera_grafico(series, ylabel, f"{teste}: {trecho}", f"{teste}_{trecho}")


if __name__ == "__main__":
    main()

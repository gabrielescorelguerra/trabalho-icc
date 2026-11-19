CFLAGS = -DLIKWID_PERFMON -I/home/soft/likwid/include
LFLAGS = -lm -L/home/soft/likwid/lib -llikwid

CC = gcc -O3 $(AVX_FLAGS)

PROG = broyden
OBJS = utils.o EliminacaoGauss.o NewtonSNLB.o SistNaoLinearBroyden.o

AVX_FLAGS = -mavx2 -march=native -fopt-info-vec

all: $(PROG)

$(PROG): $(OBJS) $(PROG).o
	$(CC) $(CFLAGS) -o $@ $^ $(LFLAGS)

$(PROG).o: $(PROG).c utils.h EliminacaoGauss.h NewtonSNLB.h SistNaoLinearBroyden.h
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(PROG) $(OBJS) $(PROG).o

CC = gcc

CFLAGS = -DLIKWID_PERFMON -I/home/soft/likwid/include
LFLAGS = -lm -L/home/soft/likwid/lib -llikwid

PROG = broyden
OBJS = utils.o EliminacaoGauss.o NewtonSNLB.o SistNaoLinearBroyden.o

all: $(PROG)

$(PROG): $(OBJS) $(PROG).o
	$(CC) $(CFLAGS) -o $@ $^ $(LFLAGS)

$(PROG).o: $(PROG).c utils.h EliminacaoGauss.h NewtonSNLB.h SistNaoLinearBroyden.h
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(PROG) $(OBJS) $(PROG).o

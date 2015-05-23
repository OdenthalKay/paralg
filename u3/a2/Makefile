# MPI compiler
MPICC	= mpicc

# no optimization for local testing only!
CFLAGS		= -std=c99 -Wall -O0 -g 
WR9_CFLAGS	= -std=c99 -Wall -O2 -g 
# libraries for all versions
LDLIBS		= -L./libFHBRS -lFHBRS
WR9_LDLIBS	= -L/usr/local/lib -lFHBRS
# default problem size
N 	= 64
# default number of processors for test run (N%P == 0!)
P	= 16

default:: a2.exe 

run:: a2.exe 
	mpirun --mca orte_base_help_aggregate 0 -np $(P) ./a2.exe $(N)
debug:: a2.exe 
	mpirun -np $(P) xterm -e gdb ./a2.exe

clean::
	-rm -f *.exe *.o

wr9:: _a2.exe
_a2.exe: _a2.o
	$(MPICC) $(WR9_CFLAGS) -o _a2.exe _a2.o $(WR9_LDLIBS)
_a2.o: a2.c
	$(MPICC) $(WR9_CFLAGS) -c a2.c -o _a2.o


a2.exe: a2.o
	$(MPICC) $(CFLAGS) -o a2.exe a2.o $(LDLIBS)
a2.o: a2.c
	$(MPICC) $(CFLAGS) -c a2.c

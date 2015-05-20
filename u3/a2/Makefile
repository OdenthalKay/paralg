# MPI compiler
MPICC	= mpicc
# compiler flags for all version
CFLAGS		= -std=c99 -Wall -O2 -g 
WR9_CFLAGS	= -std=c99 -Wall -O2 -g 
# libraries for all versions
LDLIBS		= -L./libFHBRS -lFHBRS
WR9_LDLIBS	= -L/usr/local/lib -lFHBRS
# default problem size
N 	= 10000000
# default number of processors for test run (N%P == 0!)
P	= 1

default:: a2.exe 

run:: a2.exe 
	mpirun -np $(P) ./a2.exe $(N)

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

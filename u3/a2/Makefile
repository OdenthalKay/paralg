# MPI compiler
MPICC	= mpicc
CC 		= gcc 

# no optimization for local testing only!
CFLAGS		= -std=c99 -Wall -O0 -g 
WR5_CFLAGS	= -std=c99 -Wall -O2 -g 
# libraries for all versions
LDLIBS		= -L./libFHBRS -lFHBRS
WR5_LDLIBS	= -L/usr/local/lib -lFHBRS
# default problem size for local testing
N 	= 64
# default number of processors for local test run (N%P must be equals to 0)
P	= 16

default:: local 

local:: a2.exe
run:: a2.exe 
	mpirun --mca orte_base_help_aggregate 0 -np $(P) ./a2.exe $(N)
debug:: a2.exe 
	mpirun -np $(P) xterm -e gdb ./a2.exe
clean::
	-rm -f *.exe *.o
	
wr5:: _a2.exe _a2_seq.exe

_a2.exe: _a2.o
	$(MPICC) $(WR5_CFLAGS) -o _a2.exe _a2.o $(WR5_LDLIBS)
_a2.o: a2.c
	$(MPICC) $(WR5_CFLAGS) -c a2.c -o _a2.o
_a2_seq.exe: _a2_seq.o
	$(CC) $(WR5_CFLAGS) -o _a2_seq.exe _a2_seq.o $(WR5_LDLIBS)
_a2_seq.o: a2_seq.c
	$(CC) $(WR5_CFLAGS) -c a2_seq.c -o _a2_seq.o

# testing the parallel version locally
a2.exe: a2.o
	$(MPICC) $(CFLAGS) -o a2.exe a2.o $(LDLIBS)
a2.o: a2.c
	$(MPICC) $(CFLAGS) -c a2.c

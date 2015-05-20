# MPI compiler
MPICC	= mpicc
# compiler flags for all version
CFLAGS	= -std=c99 -Wall -O2 -g 
# libraries for all versions
LDLIBS	= -L./libFHBRS -lFHBRS
# default number of processors for test run (N%P == 0!)
P	= 2

default:: a2.exe 

run:: a2.exe 
	mpirun -np $(P) ./a2.exe

clean::
	-rm -f *.exe *.o


a2.exe: a2.o
	$(MPICC) $(CFLAGS) -o $@ $^ $(LDLIBS)
a2.o: a2.c
	$(MPICC) $(CFLAGS) -c $^


CC      = gcc
LDFLAGS  = -fopenmp -lm 
RM      = rm -f

default: a2

a2: a2.o
	$(CC) -o a2 $(LDFLAGS) a2.c

clean veryclean:
	$(RM) a2
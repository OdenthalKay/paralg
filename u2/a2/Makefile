CC      = gcc
CFLAGS  = -Wall
LDFLAGS = -lm
OMPFLAG = -fopenmp 
RM      = rm -f

default: all

all: a2_seq a2_par

a2_seq: a2.c
	$(CC) $(CFLAGS) a2.c -o a2_seq $(LDFLAGS) 

a2_par: a2.c
	$(CC) $(CFLAGS) a2.c -o a2_par $(LDFLAGS) $(OMPFLAG)

clean veryclean:
	$(RM) a2_seq.o a2_par.o a2_seq a2_par
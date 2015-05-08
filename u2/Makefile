CC      = gcc
CFLAGS  = -Wall
OMPFLAG = -fopenmp 
RM      = rm -f

default: all

all: a2_seq a2_par a3_seq a3_par

a2_seq: a2_seq.c
	$(CC) $(CFLAGS) a2_seq.c -o a2_seq $(LDFLAGS) 

a2_par: a2_par.c
	$(CC) $(CFLAGS) a2_par.c -o a2_par $(LDFLAGS) $(OMPFLAG)

a3_seq: a3_seq.c
	$(CC) $(CFLAGS) a3_seq.c -o a3_seq $(LDFLAGS)

a3_par: a3_par.c
	$(CC) $(CFLAGS) a3_par.c -o a3_par $(LDFLAGS) $(OMPFLAG)

clean veryclean:
	$(RM) a2_seq a2_par a3_seq a3_par
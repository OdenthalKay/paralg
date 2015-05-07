

CC      = gcc
CFLAGS  = -g -Wall -fopenmp
LFLAGS  = -lm
RM      = rm -f

default: all

all: a2

a2: a2.o
	$(CC) -o a2 a2.o $(LFLAGS)

a2.o: a2.c
	$(CC) -c $(CFLAGS) a2.c 

clean veryclean:
	$(RM) a2
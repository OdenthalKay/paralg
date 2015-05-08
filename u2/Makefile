CC = gcc -fopenmp
LDLIBS = -L/usr/local/lib -lFHBRS
CFLAGS = -O
RM = rm -f
default: all
all: a2_par.exe
a2.o: a2_par.c
	$(CC) $(CFLAGS) -c $<
a2_par.exe: a2_par.o
	$(CC) -o $@ $< $(LDLIBS)
clean veryclean:
	$(RM) a2_seq a2_par a3_seq a3_par
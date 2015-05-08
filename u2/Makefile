CC = icc -openmp
LDLIBS = -L/usr/local/lib -lFHBRS
CFLAGS = -O
RM = rm -f

default: all
all: a2_seq.exe a2_par.exe a3_seq.exe a3_par.exe

a2_seq.o: a2_seq.c
	$(CC) $(CFLAGS) -c $<
a2_seq.exe: a2_seq.o
	$(CC) -o $@ $< $(LDLIBS)

a2_par.o: a2_par.c
	$(CC) $(CFLAGS) -c $<
a2_par.exe: a2_par.o
	$(CC) -o $@ $< $(LDLIBS)

a3_seq.o: a3_seq.c
	$(CC) $(CFLAGS) -c $<
a3_seq.exe: a3_seq.o
	$(CC) -o $@ $< $(LDLIBS)

a3_par.o: a3_par.c
	$(CC) $(CFLAGS) -c $<
a3_par.exe: a3_par.o
	$(CC) -o $@ $< $(LDLIBS)


clean veryclean:
	$(RM) *.exe *.o
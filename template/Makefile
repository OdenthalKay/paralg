CC = gcc
OMP = -fopenmp
###########################################################
# debug
DEBUG_CFLAGS= $(OMP) -g -O0 -Wall
DEBUG_INCLUDES=
DEBUG_LIBS=
# wr5
CFLAGS= $(OMP) -O3
INCLUDES=
LIBS=

# execute locally or just build for execution on wr5
default: clean debug
debug: seq_debug
	./bin/$<
wr5: seq par

# debug
seq_debug: seq_debug.o
	$(CC) $^ $(DEBUG_LIBS) -o $@
	mv $@ ./bin
seq_debug.o: seq.c
	$(CC) -c $(DEBUG_CFLAGS) $(DEBUG_INCLUDES) $^ -o $@
par_debug: par.o
	$(CC) $^ $(DEBUG_LIBS) -o $@
	mv $@ ./bin
par_debug.o: par_debug.c
	$(CC) -c $(DEBUG_CFLAGS) $(DEBUG_INCLUDES) $^ -o $@

# wr5
seq: seq.o
	$(CC) $^ $(LIBS) -o $@
	mv $@ ./wr5
seq.o: seq.c
	$(CC) -c $(CFLAGS) $(INCLUDES) $^ -o $@
par: par.o
	$(CC) $^ $(LIBS) -o $@
	mv $@ ./wr5
par.o: par.c
	$(CC) -c $(CFLAGS) $(INCLUDES) $^ -o $@

# cleanup
# .PHONY: make clean a valid target so that it can be used as a prequisite
# -: ignore non-zero code => clean cannot fail
# @: ignore rm messages
.PHONY: clean
clean:
	-@rm -rf *.o ./bin/* ./wr5/*


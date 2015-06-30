
CC     = gcc
CC_OMP = gcc -fopenmp
###########################################################
# debug
DEBUG_CFLAGS= -g -O0 -Wall -std=c99
DEBUG_INCLUDES=-I.
DEBUG_LIBS= -lm
# wr5
CFLAGS= -O3
INCLUDES=
LIBS=

# execute locally or just build for execution on wr5
default: debug
all_debug: midpoint_par_debug
debug: midpoint_par_debug
	./bin/$<
wr5: seq par

# debug
%_debug: %_debug.o
	$(CC) $^ $(DEBUG_LIBS) -o $@
	mv $@ ./bin
%_debug.o: %.c
	$(CC) -c $(DEBUG_CFLAGS) $(DEBUG_INCLUDES) $^ -o $@
%_par_debug: %_par_debug.o
	$(CC_OMP) $^ $(DEBUG_LIBS) -o $@
	mv $@ ./bin
%_par_debug.o: %.c
	$(CC_OMP) -c $(DEBUG_CFLAGS) $(DEBUG_INCLUDES) $^ -o $@

# wr5
seq: seq.o
	$(CC) $^ $(LIBS) -o $@
	mv $@ ./wr5
seq.o: seq.c
	$(CC) -c $(CFLAGS) $(INCLUDES) $^ -o $@
par: par.o
	$(CC_OMP) $^ $(LIBS) -o $@
	mv $@ ./wr5
par.o: par.c
	$(CC_OMP) -c $(CFLAGS) $(INCLUDES) $^ -o $@

# cleanup
# .PHONY: make clean a valid target so that it can be used as a prequisite
# -: ignore non-zero code => clean cannot fail
# @: ignore rm messages

clean:
	-@rm -rf *.o ./bin/* ./wr5/*


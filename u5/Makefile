
CC     = icc
CC_OMP = icc -openmp
###########################################################
# debug
DEBUG_CFLAGS= -g -O0 -Wall -std=c99
DEBUG_INCLUDES=
DEBUG_LIBS= -lm -lFHBRS
# wr5
CFLAGS= -std=c99 -Wall -O2 -g
INCLUDES= 
LIBS= -L/usr/local/lib -lFHBRS

# execute locally or just build for execution on wr5
default: debug
all_debug: midpoint_par_debug trapezoid_par_debug simpson_par_debug montecarlo_par_debug
debug: montecarlo_par_debug
	./bin/$< 100
all_wr5: midpoint_par 

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
%_seq: %_seq.o
	$(CC) $^ $(LIBS) -o $@
	mv $@ ./wr5
%_seq.o: %.c
	$(CC) -c $(CFLAGS) $(INCLUDES) $^ -o $@
%_par: %_par.o
	$(CC_OMP) $^ $(LIBS) -o $@
	mv $@ ./wr5
%_par.o: %.c
	$(CC_OMP) -c $(CFLAGS) $(INCLUDES) $^ -o $@

# cleanup
# .PHONY: make clean a valid target so that it can be used as a prequisite
# -: ignore non-zero code => clean cannot fail
# @: ignore rm messages

clean:
	-@rm -rf *.o ./bin/* ./wr5/*


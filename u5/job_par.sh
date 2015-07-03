#!/bin/sh
#PBS -q wr5
#PBS -l nodes=1:ppn=56
#PBS -l walltime=00:20:00
#PBS -l vmem=16GB
#PBS -V 

module load intel-icc/default

# change to submit directory (with executable)
cd $PBS_O_WORKDIR

#sequential versions
echo "--- SEQUENTIAL VERSIONS ---"
for EXP in 0 1 2 3 4 5 6 ; do
	N=$(echo 10^$EXP | bc)
	./wr5/midpoint_seq $N
done
for EXP in 0 1 2 3 4 5 6 ; do
	N=$(echo 10^$EXP | bc)
	./wr5/trapezoid_seq $N
done
for EXP in 0 1 2 3 4 5 6 ; do
	N=$(echo 10^$EXP | bc)
	./wr5/simpson_seq $N
done
for EXP in 0 1 2 3 4 5 6 ; do
	N=$(echo 10^$EXP | bc)
	./wr5/montecarlo_seq $N
done

# parallel versions
echo "--- PARALLEL VERSIONS ---"
echo "--- MIDPOINT ---"
for EXP in 5 6 7 8 9 ; do
	N=$(echo 10^$EXP | bc)
	for THREADS in 1 2 4 8 16 32 48 56 ; do
		export OMP_NUM_THREADS=$THREADS
		./wr5/midpoint_par $N $THREADS
	done
done
echo "--- MIDPOINT ---"

echo "--- TRAPEZOID ---"
for EXP in 5 6 7 8 9 ; do
	N=$(echo 10^$EXP | bc)
	for THREADS in 1 2 4 8 16 32 48 56 ; do
		export OMP_NUM_THREADS=$THREADS
		./wr5/trapezoid_par $N $THREADS
	done
done
echo "--- TRAPEZOID ---"

echo "--- SIMPSON ---"
for EXP in 5 6 7 8 9 ; do
	N=$(echo 10^$EXP | bc)
	for THREADS in 1 2 4 8 16 32 48 56 ; do
		export OMP_NUM_THREADS=$THREADS
		./wr5/simpson_par $N $THREADS
	done
done
echo "--- SIMPSON ---"

echo "--- MONTECARLO ---"
for EXP in 5 6 7 8 9 ; do
	N=$(echo 10^$EXP | bc)
	for THREADS in 1 2 4 8 16 32 48 56 ; do
		export OMP_NUM_THREADS=$THREADS
		./wr5/montecarlo_par $N $THREADS
	done
done
echo "--- MONTECARLO ---"

exit
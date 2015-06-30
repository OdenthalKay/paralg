#!/bin/sh
#PBS -q wr5
#PBS -l nodes=1:ppn=16
#PBS -l walltime=00:10:00
#PBS -l vmem=16GB
#PBS -V 
www.google.de

module load intel-icc/default

# change to submit directory (with executable)
cd $PBS_O_WORKDIR

echo "N = " $N ":"
for EXP in 10 11 12 ; do
	N=$(echo 2^$EXP | bc)
	for THREADS in 1 2 4 ; do
		export OMP_NUM_THREADS=$THREADS
		./seq
	done
done

exit
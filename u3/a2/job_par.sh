#!/bin/sh
#PBS -q mpi
#PBS -l nodes=8:ppn=16
#PBS -l walltime=00:10:00
#PBS -l vmem=16GB
#PBS -N paralg_u3_a2_par

# change to job submit directory
cd $PBS_O_WORKDIR

# load openmpi module
module load openmpi/gnu

echo "N = " $N ":"
for EXP in 20 21 22 23 24 25 26 27 28 ; do
	N=$(echo 2^$EXP | bc)
	for NCORES in 1 2 4 8 16 32 64 128 ; do
		mpirun -np $NCORES -machinefile $PBS_NODEFILE _a2.exe $N
	done
done

exit

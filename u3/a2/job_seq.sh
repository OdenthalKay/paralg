#!/bin/sh
#PBS -q mpi
#PBS -l nodes=1:ppn=16
#PBS -l walltime=00:10:00
#PBS -l vmem=16GB
#PBS -N paralg_u3__a2_seq

# change to job submit directory
cd $PBS_O_WORKDIR

# load openmpi module
module load openmpi/gnu

for EXP in 20 21 22 23 24 25 26 27 28 ; do
	N=$(echo 2^$EXP | bc)
	./_a2_seq.exe $N 
done

exit

#!/bin/sh
# 1 node with 16 cores used, 2 minutes maximum runtime
#PBS -q wr5
#PBS -l nodes=1:ppn=16
#PBS -l walltime=00:02:00
#PBS -l vmem=1GB

# change to submit directory (with executable)
cd $PBS_O_WORKDIR
# execute parallel OpenMP program with 16 threads
export OMP_NUM_THREADS=16
./a2.exe
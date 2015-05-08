#!/bin/sh
# 1 node with 16 cores used, 2 minutes maximum runtime
#PBS -q wr5
#PBS -l nodes=1:ppn=16
#PBS -l walltime=00:02:00
#PBS -l vmem=16GB
#PBS -V 


module load intel-icc/default

# change to submit directory (with executable)
cd $PBS_O_WORKDIR
# execute parallel OpenMP program with 16 threads
n1=$(echo 2^20 | bc)
n2=$(echo 2^21 | bc)
n3=$(echo 2^22 | bc)
n4=$(echo 2^23 | bc)
n5=$(echo 2^24 | bc)
n6=$(echo 2^25 | bc)
n7=$(echo 2^26 | bc)
n8=$(echo 2^27 | bc)
export OMP_NUM_THREADS=1
./a2_par.exe $n1
./a2_par.exe $n2




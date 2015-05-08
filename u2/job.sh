#!/bin/sh
#PBS -q wr5
#PBS -l nodes=1:ppn=32
#PBS -l walltime=00:20:00
#PBS -l vmem=16GB
#PBS -V 

module load intel-icc/default

# change to submit directory (with executable)
cd $PBS_O_WORKDIR

# execute parallel OpenMP programs
n1=$(echo 2^20 | bc)
n2=$(echo 2^21 | bc)
n3=$(echo 2^22 | bc)
n4=$(echo 2^23 | bc)
n5=$(echo 2^24 | bc)
n6=$(echo 2^25 | bc)
n7=$(echo 2^26 | bc)
n8=$(echo 2^27 | bc)
n9=$(echo 2^28 | bc)
#execute sequential programs
export OMP_NUM_THREADS=1
### A2
./a2_seq.exe $n1
./a2_seq.exe $n2
./a2_seq.exe $n3
./a2_seq.exe $n4
./a2_seq.exe $n5
./a2_seq.exe $n6
./a2_seq.exe $n7
./a2_seq.exe $n8
./a2_seq.exe $n9
### A3
./a3_seq.exe $n1
./a3_seq.exe $n2
./a3_seq.exe $n3
./a3_seq.exe $n4
./a3_seq.exe $n5
./a3_seq.exe $n6
./a3_seq.exe $n7
./a3_seq.exe $n8
./a3_seq.exe $n9

# execute parallel programs
export OMP_NUM_THREADS=1
./a2_par.exe $n1
./a2_par.exe $n2
./a2_par.exe $n3
./a2_par.exe $n4
./a2_par.exe $n5
./a2_par.exe $n6
./a2_par.exe $n7
./a2_par.exe $n8
./a2_par.exe $n9
export OMP_NUM_THREADS=2
./a2_par.exe $n1
./a2_par.exe $n2
./a2_par.exe $n3
./a2_par.exe $n4
./a2_par.exe $n5
./a2_par.exe $n6
./a2_par.exe $n7
./a2_par.exe $n8
./a2_par.exe $n9
export OMP_NUM_THREADS=4
./a2_par.exe $n1
./a2_par.exe $n2
./a2_par.exe $n3
./a2_par.exe $n4
./a2_par.exe $n5
./a2_par.exe $n6
./a2_par.exe $n7
./a2_par.exe $n8
./a2_par.exe $n9
export OMP_NUM_THREADS=8
./a2_par.exe $n1
./a2_par.exe $n2
./a2_par.exe $n3
./a2_par.exe $n4
./a2_par.exe $n5
./a2_par.exe $n6
./a2_par.exe $n7
./a2_par.exe $n8
./a2_par.exe $n9
export OMP_NUM_THREADS=16
./a2_par.exe $n1
./a2_par.exe $n2
./a2_par.exe $n3
./a2_par.exe $n4
./a2_par.exe $n5
./a2_par.exe $n6
./a2_par.exe $n7
./a2_par.exe $n8
./a2_par.exe $n9
export OMP_NUM_THREADS=32
./a2_par.exe $n1
./a2_par.exe $n2
./a2_par.exe $n3
./a2_par.exe $n4
./a2_par.exe $n5
./a2_par.exe $n6
./a2_par.exe $n7
./a2_par.exe $n8
./a2_par.exe $n9
###############################################
export OMP_NUM_THREADS=1
./a3_par.exe $n1
./a3_par.exe $n2
./a3_par.exe $n3
./a3_par.exe $n4
./a3_par.exe $n5
./a3_par.exe $n6
./a3_par.exe $n7
./a3_par.exe $n8
./a3_par.exe $n9
export OMP_NUM_THREADS=2
./a3_par.exe $n1
./a3_par.exe $n2
./a3_par.exe $n3
./a3_par.exe $n4
./a3_par.exe $n5
./a3_par.exe $n6
./a3_par.exe $n7
./a3_par.exe $n8
./a3_par.exe $n9
export OMP_NUM_THREADS=4
./a3_par.exe $n1
./a3_par.exe $n2
./a3_par.exe $n3
./a3_par.exe $n4
./a3_par.exe $n5
./a3_par.exe $n6
./a3_par.exe $n7
./a3_par.exe $n8
./a3_par.exe $n9
export OMP_NUM_THREADS=8
./a3_par.exe $n1
./a3_par.exe $n2
./a3_par.exe $n3
./a3_par.exe $n4
./a3_par.exe $n5
./a3_par.exe $n6
./a3_par.exe $n7
./a3_par.exe $n8
./a3_par.exe $n9
export OMP_NUM_THREADS=16
./a3_par.exe $n1
./a3_par.exe $n2
./a3_par.exe $n3
./a3_par.exe $n4
./a3_par.exe $n5
./a3_par.exe $n6
./a3_par.exe $n7
./a3_par.exe $n8
./a3_par.exe $n9
export OMP_NUM_THREADS=32
./a3_par.exe $n1
./a3_par.exe $n2
./a3_par.exe $n3
./a3_par.exe $n4
./a3_par.exe $n5
./a3_par.exe $n6
./a3_par.exe $n7
./a3_par.exe $n8
./a3_par.exe $n9
export OMP_NUM_THREADS=48
./a3_par.exe $n1
./a3_par.exe $n2
./a3_par.exe $n3
./a3_par.exe $n4
./a3_par.exe $n5
./a3_par.exe $n6
./a3_par.exe $n7
./a3_par.exe $n8
./a3_par.exe $n9
export OMP_NUM_THREADS=56
./a3_par.exe $n1
./a3_par.exe $n2
./a3_par.exe $n3
./a3_par.exe $n4
./a3_par.exe $n5
./a3_par.exe $n6
./a3_par.exe $n7
./a3_par.exe $n8
./a3_par.exe $n9




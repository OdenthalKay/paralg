#!/bin/sh
#PBS -q mpi
#PBS -l nodes=1:ppn=16
#PBS -l walltime=00:10:00
#PBS -l vmem=16GB
#PBS -N paralg_u3_a2_par

# change to job submit directory
cd $PBS_O_WORKDIR

# load openmpi module
module load openmpi/gnu

n1=$(echo 2^20 | bc)
n2=$(echo 2^21 | bc)
n3=$(echo 2^22 | bc)
n4=$(echo 2^23 | bc)
n5=$(echo 2^24 | bc)
n6=$(echo 2^25 | bc)
n7=$(echo 2^26 | bc)
n8=$(echo 2^27 | bc)
n9=$(echo 2^28 | bc)

# following the actions to be performed by the job
# start here your MPI program on the nodes we got assigned from the batch system
for NCORES in 4 8 16 ; do

echo "running with " $NCORES " cores"

mpirun -np $NCORES -machinefile $PBS_NODEFILE _a2.exe $n1
mpirun -np $NCORES -machinefile $PBS_NODEFILE _a2.exe $n2
mpirun -np $NCORES -machinefile $PBS_NODEFILE _a2.exe $n3
mpirun -np $NCORES -machinefile $PBS_NODEFILE _a2.exe $n4
mpirun -np $NCORES -machinefile $PBS_NODEFILE _a2.exe $n5
mpirun -np $NCORES -machinefile $PBS_NODEFILE _a2.exe $n6
mpirun -np $NCORES -machinefile $PBS_NODEFILE _a2.exe $n7
mpirun -np $NCORES -machinefile $PBS_NODEFILE _a2.exe $n8
mpirun -np $NCORES -machinefile $PBS_NODEFILE _a2.exe $n9

done

exit

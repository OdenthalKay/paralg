#!/bin/sh
#PBS -q mpi
#PBS -l nodes=1:ppn=16
#PBS -l walltime=00:01:00
#PBS -l vmem=16GB
#PBS -N paralg_u3_a2_par

# change to job submit directory
cd $PBS_O_WORKDIR

# load openmpi module
module load openmpi/gnu

# matrix dimension
export N=10

# following the actions to be performed by the job
# start here your MPI program on the nodes we got assigned from the batch system
NCORES=`cat $PBS_NODEFILE | wc -l`
mpirun -np $NCORES -machinefile $PBS_NODEFILE _a2.exe $N

done

exit

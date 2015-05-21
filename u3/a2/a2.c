#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "libFHBRS/libFHBRS.h"

int cmpfunc(const void *a, const void *b);
int is_sorted(int x[], int length);

int main(int argc, char** argv) {
  if (argc < 2) {
    printf("Wrong number of arguments. Expecting: <n>\n");
    return -1;
  }
  int n = atoi(argv[1]);
  int i, blocksize, rank, num_processors;
  int *values;
  int *block;
  double t0, t1;
  ////////////////////////////////////////////////////////
  t0 = gettime();
  MPI_Init(NULL, NULL);
  MPI_Comm_size(MPI_COMM_WORLD, &num_processors);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  blocksize = n/num_processors;

  // 1. allocate memory and fill array values in reverse order
  // 2. split values among all processors
  if (rank == 0) {
    values = malloc(n*sizeof(int));
    for (i=0;i<n;i++) {
      values[i] = n-i;
    }

    int *start = &values[blocksize];
    for (i=1;i<num_processors;i++) {
      MPI_Send(start, blocksize, MPI_INT, i, 0, MPI_COMM_WORLD);
      start += blocksize;
    }
    qsort(values, blocksize, sizeof(int), cmpfunc);
    assert(is_sorted(values,blocksize));
  } else {
    block = malloc(blocksize*sizeof(int));
    MPI_Recv(block, blocksize, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    qsort(block, blocksize, sizeof(int), cmpfunc);
    assert(is_sorted(block, blocksize));
  }

  if (rank == 0) {
    t1 = gettime();
    printf("\nCalculation took %f seconds.\n",t1-t0);
  }
  MPI_Finalize();
  return 0;
}

///////////////////////////////////////////////////////////

/* 
returns negative if b > a 
and positive if a > b 
*/  
int cmpfunc(const void *a, const void *b) { 
    const int *x = (const int *)a; // casting pointer types 
    const int *y = (const int *)b;
    return *x  - *y;  
  } 

int is_sorted(int x[], int length) {
  int i;
  for (i=0;i<length-1;i++) {
    if (x[i+1] < x[i]) {
      return 0;
    }
  }
  return 1;
}

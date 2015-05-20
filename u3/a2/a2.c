#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include "libFHBRS/libFHBRS.h"

void printArray(int a[], int length);
int cmpfunc(const void *a, const void *b);

int main(int argc, char** argv) {
    if (argc < 2) {
      printf("Wrong number of arguments. Expecting: <n>\n");
      return -1;
    }
    int n = atoi(argv[1]);
    double t0, t1;
    int i;
    int *values = malloc(n*sizeof(int));
      
    // fill array in reverse order     
    for (i=0;i<n;i++) {
      values[i] = n-i;
    }
    
    t0 = gettime();

   //printf("Before sorting the list is: \n");
   //printArray(values,n);
   qsort(values, n, sizeof(int), cmpfunc);
   //printf("After sorting the list is: \n");
   //printArray(values,n);
   
    // Initialize the MPI environment
    MPI_Init(NULL, NULL);

    // Get the number of processes
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    // Get the rank of the process
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    // Get the name of the processor
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    int name_len;
    MPI_Get_processor_name(processor_name, &name_len);

    t1 = gettime();
    printf("\nCalculation took %f seconds.\n",t1-t0);
    // Finalize the MPI environment.
    MPI_Finalize();
    return 0;
}

////////////////////////////////////////////////////////////////
void printArray(int a[], int length) {
   int i;
   for(i=0;i<length;i++) {
      printf("%d ", a[i]);
   }
   printf("\n");
}

/* 
returns negative if b > a 
and positive if a > b 
*/  
int cmpfunc(const void *a, const void *b) { 
    const int *x = (const int *)a; // casting pointer types 
    const int *y = (const int *)b;
    return *x  - *y;  
} 

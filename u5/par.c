#include "dgb.h"
#include <stdlib.h>
#include <stdio.h>
#include <omp.h>

int main(int argc, char *argv[])
{
	int num_threads = 2;

	omp_set_num_threads(num_threads);
	#pragma omp parallel
	{
		check(omp_get_num_threads() == num_threads, "is the -fopenmp flag set?");
		int rank = omp_get_thread_num();
		printf("I am thread %d\n", rank);
	}

	return 0;
}


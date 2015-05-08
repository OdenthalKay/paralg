#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include <libFHBRS.h>
#include "helper.c"

int main(int argc, char *argv[])
{
	if(argc != 2) {
		printf("Wrong number of arguments. Expecting: './program <n>'\n");
		return 1;
	}
	int n = atoi(argv[1]);
	int p;
	atype_t *x = malloc(n*sizeof(atype_t));
	atype_t *y = malloc(n*sizeof(atype_t));
	atype_t *prefix1 = malloc(n*sizeof(atype_t));
	atype_t *prefix2;
	atype_t *adjust; // prefix of last block elements
	double t0, t1;

	fill_array(x,n);
	#pragma omp parallel
	{
		#pragma omp single
		{
			p = omp_get_num_threads();
			prefix2 = malloc(p*sizeof(atype_t));
			adjust =  malloc(p*sizeof(atype_t));

		}
		#pragma omp barrier

		int blocksize = n/p;
		int myproc = omp_get_thread_num();
		int start = myproc * blocksize;
		atype_t *results = malloc(blocksize*sizeof(atype_t));
		int i;

		#pragma omp single
		t0 = gettime();
		
		// insert calculated values
		sequential_prefix(x,start,blocksize,results);	
		for (i=0;i<blocksize;i++) {
			prefix1[start+i] = results[i];
		}
		#pragma omp barrier

		// Do a prefix operation on last elements from previous block
		if (myproc == 0) {
			adjust[myproc] = 0; // the first processor has no previous block
		} else {
			adjust[myproc] = prefix1[start-1];
		}
		#pragma omp barrier
		

		#pragma omp single 
		sequential_prefix(adjust, 0, p, prefix2);


		// replace first value in each block with corresponding last block prefix + first value
		x[start] = x[start] + prefix2[myproc];
		sequential_prefix(x,start,blocksize,results);
		for (i=0;i<blocksize;i++) {
			y[start+i] = results[i];
		}
		#pragma omp barrier

	}
	t1 = gettime();
	printf("p: %d\nn: %d\nseconds: %.6f\n",p,n,t1-t0);
	return 0;
}


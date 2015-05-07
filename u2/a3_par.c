#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include "a3_helper.c"

int main(int argc, char *argv[])
{
	if(argc != 2) {
		printf("Wrong number of arguments. Expecting: './program <n> <p>'\n");
		return 1;
	}
	int n = atoi(argv[1]);
	int p = atoi(argv[2]);
	double x[n];
	double y[n];
	double prefix1[n];
	double prefix2[p];
	double adjust[p]; // prefix of last block elements

	omp_set_num_threads(p);
	fill_array(x,n);
	#pragma omp parallel
	{
		int blocksize = n/p;
		int myproc = omp_get_thread_num();
		int start = myproc * blocksize;
		double results[blocksize];
		int i;

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

		/*
		Calculate prefix of values that were previously not assigned to a block.
		This happens when n mod p isn't 0.
		*/
		#pragma omp single
		{
			int rest = n % p;
			int rest_blocksize = rest+1;
			double tmp[rest_blocksize];  // input
			double tmp2[rest_blocksize]; // output

			/*
			fill array with: 
			- last calculated prefix value
			- values of x that were not covered yet 
			*/
			tmp[0] = y[n-rest-1];
			for (i=0;i<rest;i++) {
				tmp[i+1] = x[n-rest_blocksize];
			}
			sequential_prefix(tmp,0,rest_blocksize,tmp2);

			// write prefixes to the result array
			for (i=0;i<rest_blocksize;i++) {
				y[n-rest_blocksize+i] = tmp2[i];
			}
		}
		print_array(myproc,y,n);
	}

	return 0;
}


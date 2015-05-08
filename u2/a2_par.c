
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <libFHBRS.h>
#include "helper.c"

/*
This file is used for both the sequential and parallel version.
The sequential version doesn't use openmp pragmas.
*/

void balanced_tree(int n1, atype_t values[n1], atype_t (*f)(atype_t x, atype_t y));

int main(int argc, char *argv[])
{
	if(argc != 2) {
		printf("Wrong number of arguments. Expecting: './program <n>'\n");
		return 1;
	}
	printf("===== a2_par.exe ===\n");
	int n = atoi(argv[1]);
	int p;
	int n1 = 2*n-1; // leafs + inner elements + root
	atype_t *values = malloc(n1*sizeof(atype_t));
	int i;
	double t0, t1;

	// fill array with values 1...n 
	int x = 1;
	for (i=n1-n; i<n1; i++) {
		values[i] = x;
		x++;
	}
	#pragma omp parallel
	{
		#pragma omp single
		{
			p = omp_get_num_threads();
			t0 = gettime();
		}
		
		balanced_tree(n1,values,sum);
		
		#pragma omp single
		{
			t1 = gettime();
			printf("p: %d\nn: %d\nseconds: %.6f\n",p,n,t1-t0);
		}
	}
	return 0;
}

/////////////////////////////////////////////////////////////

/*
Reduction with intermediate results
*/
void balanced_tree(int n1, atype_t values[n1], atype_t (*f)(atype_t x, atype_t y)) {
	int leafs = n1/2+1;
	int stride, i; 
	for (stride=(leafs/2); stride>0; stride/=2) {
		#pragma omp for
		for (i=0; i<stride; i++) {
			values[stride+i-1] = f(values[2*stride+i-1], values[3*stride+i-1]);
		}
	}
}


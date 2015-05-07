
#include <stdio.h>
#include <math.h>

typedef double atype_t;

atype_t sum(atype_t x, atype_t y) {
	return x+y;
}

atype_t balanced_tree(int n1, atype_t values[n1], atype_t (*f)(atype_t x, atype_t y)) {
	int leafs = log2(n1);
	int stride, i; 
	for (stride=leafs/2;stride>0;stride/=2) {
		for (i=0;i<stride;i++) {
			values[i] = f(values[i], values[i+stride]);
		}
	}
}

int main()
{
	int n = 16;
	int n1 = 2*n-1;
	int p = n/log2(n);
	atype_t a[16] = {0.0,1.0,2.0,3.0,4.0,5.0,6.0,7.0,8.0,9.0,10.0,11.0,12.0,13.0,14.0,15.0};
	atype_t b[16] = {0};
	int i, j, stride;

	// Calculate partial sums (1 sum for each processor)
	for (i=0;i<p;i++) {
		for (j=0;j<log2(n);j++) {
			b[i] += a[i*4+j];
		}
	}
	print_array(0,b,p);


	balanced_tree(n1,b,sum);
	// Calculate final sum using the binary tree template algorithm
	/*
	for (stride=4/2;stride>0;stride/=2) {
		for (i=0;i<stride;i++) {
			b[i] = sum(b[i], b[i+stride]);
		}
	}
	*/
	print_array(0,b,4);


	return 0;
}



void print_array(int threadID, double x[], int size) {
	printf("--- Array of thread with ID %d ---\n", threadID);
	int i;
	for (i=0;i<size;i++) {
		printf("%d: %.2f\n",i,x[i]);
	}
};
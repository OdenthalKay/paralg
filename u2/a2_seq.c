
#include <stdio.h>
#include <stdlib.h>
#include <libFHBRS.h>
#include "helper.c"

int main(int argc, char *argv[])
{
	if(argc != 2) {
		printf("Wrong number of arguments. Expecting: './program <n>'\n");
		return 1;
	}
	int n = atoi(argv[1]);
	atype_t *values = malloc(n*sizeof(atype_t));
	atype_t result = 0.0;
	double t0, t1;
	int i;

	fill_array(values,n);
	
	// calculate sum
	t0 = gettime();
	for (i=0;i<n;i++) {
		result = sum(result, values[i]);
	}
	t1 = gettime();
	printf("(n: %d) Calculation took %.6f seconds.\n",n,t1-t0);
	printf("SUM: %.2f\n",result);
	return 0;
}



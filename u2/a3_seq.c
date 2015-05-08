
#include <stdlib.h>
#include <stdio.h>
#include <libFHBRS.h>
#include "helper.c"

int main(int argc, char *argv[])
{
	if(argc != 2) {
		printf("Wrong number of arguments. Expecting: './program <n>'\n");
		return 1;
	}
	int n = atoi(argv[1]);
	atype_t *x = malloc(n*sizeof(atype_t));
	atype_t *y = malloc(n*sizeof(atype_t));
	double t0, t1;

	fill_array(x,n);
	//print_array(0,x,n);
	t0 = gettime();
	sequential_prefix(x, 0, n, y);
	t1 = gettime();
	printf("(n: %d) Calculation took %.6f seconds.\n",n,t1-t0);
	printf("SUM: %.2f\n",y[0]);
	//print_array(0,y,n);
	return 0;
}
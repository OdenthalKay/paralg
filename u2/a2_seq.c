
#include <stdio.h>
#include <stdlib.h>
#include "helper.c"

int main(int argc, char *argv[])
{
	if(argc != 2) {
		printf("Wrong number of arguments. Expecting: './program <n>'\n");
		return 1;
	}

	int n = atoi(argv[1]);
	atype_t values[n];
	atype_t result = 0.0;
	int i;

	fill_array(values,n);
	
	// calculate sum
	for (i=0;i<n;i++) {
		result = sum(result, values[i]);
	}

	printf("Sum: %.2f\n",result);
	return 0;
}



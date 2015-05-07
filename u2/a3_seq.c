
#include <stdlib.h>
#include <stdio.h>
#include "helper.c"

int main(int argc, char *argv[])
{
	if(argc != 2) {
		printf("Wrong number of arguments. Expecting: './program <n>'\n");
		return 1;
	}
	int n = atoi(argv[1]);
	atype_t x[n];
	atype_t y[n];

	fill_array(x,n);
	print_array(0,x,n);
	sequential_prefix(x, 0, n, y);
	print_array(0,y,n);
	return 0;
}
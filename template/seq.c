#include "dgb.h"
#include <stdlib.h>
#include <stdio.h>


int multiply(int x, int y)
{
	check(x > 0 && y > 0, "arguments must not be negative!");
	return x * y;
}


int main(int argc, char *argv[])
{

	check(24 == 5, "should be 25");
	printf("hallo\n");
	return 0;
}


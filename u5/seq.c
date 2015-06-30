
/*
* TRAPEZOID
*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double trapezoid(int a, int b, int n) {
	double h = (double)(b - a) / n;
	double approx = 0;

	return approx;
}

int main(int argc, char const *argv[])
{
	double approx, exact;
	int a = 0;
	int b = 1000;
	int n = 100000000;

	printf("--- trapezoid: sequential integral ---\n");
	approx = trapezoid(a, b, n);
	exact  = (-cos(b)) - (-cos(a));
	printf("trapezoid approx: %.5f\n", approx);
	printf("exact: %.5f\n",  exact);
	return 0;
}



/*
* TRAPEZOID
*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

static double f(double x) { return sin(x); }

double simpson(double a, double b, int n) {
	double h = (b - a) / n;
	double approx;

	

	return approx;
}

int main(int argc, char const *argv[])
{
	double approx, exact;
	int a = 0;
	int b = 1000;
	int n = 10000;

	printf("--- trapezoid: sequential integral ---\n");
	approx = simpson(a, b, n);
	exact  = (-cos(b)) - (-cos(a));
	printf("trapezoid approx: %.5f\n", approx);
	printf("exact: %.5f\n",  exact);
	return 0;
}


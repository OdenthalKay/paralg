
/*
* TRAPEZOID
*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

static double f(double x) { return sin(x); }

double trapezoid(double a, double b, int n) {
	double h = (b - a) / n;
	double approx;

	/* special case: first and last function value */
	approx = (f(a) + f(b)) / 2.0;

	/* all other values */
	for (int i = 1; i < n; i++) {
		double x = a + i * h;
		approx += f(x);
	}

	return approx*h;
}

int main(int argc, char const *argv[])
{
	double approx, exact;
	int a = 0;
	int b = 1000;
	int n = 10000;

	printf("--- trapezoid: sequential integral ---\n");
	approx = trapezoid(a, b, n);
	exact  = (-cos(b)) - (-cos(a));
	printf("trapezoid approx: %.5f\n", approx);
	printf("exact: %.5f\n",  exact);
	return 0;
}



/*
* TRAPEZOID
*/
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <libFHBRS.h>


static double f(double x) { return sin(x); }

double trapezoid(double a, double b, int n) {
	double h = (b - a) / n;
	double approx;

	/* special case: first and last function value */
	approx = (f(a) + f(b)) / 2.0;

	/* all other values */
	#pragma omp parallel for reduction(+:approx)
	for (int i = 1; i < n; i++) {
		double x = a + i * h;
		approx += f(x);
	}

	return approx*h;
}

int main(int argc, char const *argv[])
{
	assert(argc == 2);
	double approx, exact, t1, t2;
	int n = atoi(argv[1]);
	int a = 0;
	int b = 1000;

	t1 = gettime();
	approx = trapezoid(a, b, n);
	t2 = gettime();
	printf("calculation took %.5f seconds\n", t2 - t1);
	exact  = (-cos(b)) - (-cos(a));
	printf("approx: %.5f\n", approx);
	printf("exact: %.5f\n",  exact);
	return 0;
}


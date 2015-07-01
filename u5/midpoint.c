/*
* MIDPOINT
*/
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <libFHBRS.h>

static double f(double x) { return sin(x); }

double midpoint(int a, int b, int n) {
	double h = (double)(b - a) / n;
	double approx = 0;

	#pragma omp parallel for reduction(+:approx)
	for (int i = 0; i <= (n / 2); i++) {
		int index = i * 2;
		double x = a + index * h;
		approx += f(x);
	}

	return 2*h*approx;
}

int main(int argc, char const *argv[])
{
	assert(argc == 2);
	double approx, exact, t1, t2;
	int n = atoi(argv[1]);
	int a = 0;
	int b = 1000;

	t1 = gettime();
	approx = midpoint(a, b, n);
	t2 = gettime();
	printf("calculation took %.5f seconds\n", t2 - t1);
	exact  = (-cos(b)) - (-cos(a));
	printf("approx: %.5f\n", approx);
	printf("exact: %.5f\n",  exact);
	return 0;
}
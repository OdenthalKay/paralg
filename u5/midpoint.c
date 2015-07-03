/*
* MIDPOINT
*/
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <libFHBRS.h>
#include <omp.h>

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
	assert(argc == 3);
	double approx, exact, relative_error, t1, t2;
	int n = atoi(argv[1]);
	int p = atoi(argv[2]);
	int a = 0;
	int b = 1000;

	omp_set_num_threads(p);
	t1 = gettime();
	approx = midpoint(a, b, n);
	t2 = gettime();
	exact  = (-cos(b)) - (-cos(a));
	relative_error = (approx - exact)/exact;
	printf("%.5f %.5f %.5f\n", t2 - t1, approx, relative_error);
	return 0;
}
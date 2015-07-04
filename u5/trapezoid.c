
/*
* TRAPEZOID
*/
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <libFHBRS.h>
#include <omp.h>

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
	assert(argc == 3);
	double approx, exact, relative_error, t1, t2;
	int n = atoi(argv[1]);
	int p = atoi(argv[2]);
	int a = 0;
	int b = 1000;

	#ifdef USE_OMP
	omp_set_num_threads(p);
	#endif
	
	t1 = gettime();
	approx = trapezoid(a, b, n);
	t2 = gettime();
	exact  = (-cos(b)) - (-cos(a));
	relative_error = fabs((approx - exact)/exact);
	printf("%.5f %.5f %.5f\n", t2 - t1, approx, relative_error);
	return 0;
}


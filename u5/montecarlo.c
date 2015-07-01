
/*
* MONTECARLO
*/
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <libFHBRS.h>

static double f(double x) { return sin(x); }

double montecarlo(int a, int b, int n) {
	double approx = 0;

	/* generate n random numbers in parallel */
	void *generator  =  randp_init(0L);
	double random_numbers[n];
	randp_vector(generator, n, random_numbers);

	#pragma omp parallel for reduction(+:approx)
	for (int i = 0; i < n; i++) {
		double r = random_numbers[i] * b;
		approx += f(r);
	}
	return (approx / n) * (b - a);
}

int main(int argc, char const *argv[])
{
	assert(argc == 2);
	double approx, exact, t1, t2;
	int n = atoi(argv[1]);
	int a = 0;
	int b = 1000;

	t1 = gettime();
	approx = montecarlo(a, b, n);
	t2 = gettime();
	printf("calculation took %.5f seconds\n", t2 - t1);
	exact  = (-cos(b)) - (-cos(a));
	printf("approx: %.5f\n", approx);
	printf("exact: %.5f\n",  exact);
	return 0;
}


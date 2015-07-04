
/*
* MONTECARLO
*/
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <libFHBRS.h>
#include <omp.h>


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
	approx = montecarlo(a, b, n);
	t2 = gettime();
	exact  = (-cos(b)) - (-cos(a));
	relative_error = fabs((approx - exact)/exact);
	printf("%.5f %.5f %.5f\n", t2 - t1, approx, relative_error);
	return 0;
}


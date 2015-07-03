
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

double simpson(double a, double b, int n) {
	assert((n % 2) == 0 && "n must be even!\n");
	double h = (b - a) / n;
	double s1 = 0;
	double s2 = 0;
	double approx = f(a) + f(b);

	#pragma omp parallel
	{
		/* odd indices */
		#pragma omp for reduction(+:s1)
		for (int i = 0; i < (n / 2); i++) {
			int index = 2 * i + 1;
			double x = a + index * h;
			s1 += f(x);
		}
		printf("%f\n", s1);

		/* even indices */
		#pragma omp for reduction(+:s2)
		for (int i = 1; i < (n / 2) - 1; i++) {
			int index = 2 * i;
			double x = a + index * h;
			s2 += f(x);
		}
	}

	approx += (4 * s2 + 2 * s1);
	return h / 3 * approx;
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
	approx = simpson(a, b, n);
	t2 = gettime();
	exact  = (-cos(b)) - (-cos(a));
	relative_error = (approx - exact)/exact;
	printf("%.5f %.5f %.5f\n", t2 - t1, approx, relative_error);
	return 0;
}


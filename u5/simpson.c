
/*
* TRAPEZOID
*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

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
	double approx, exact;
	int a = 0;
	int b = 1000;
	int n = 1000000;

	printf("--- simpson: sequential integral ---\n");
	approx = simpson(a, b, n);
	exact  = (-cos(b)) - (-cos(a));
	printf("trapezoid approx: %.5f\n", approx);
	printf("exact: %.5f\n",  exact);
	return 0;
}


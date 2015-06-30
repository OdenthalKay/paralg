
/*
* MONTECARLO
*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

static double f(double x) { return sin(x); }

double montecarlo(int a, int b, int n) {
	double approx = 0;	

	srand(time(NULL));
	for (int i = 0; i < n; i++) {
		double r = rand() % b;
		approx += f(r);
	}
	return (approx/n)*(b-a);
}

int main(int argc, char const *argv[])
{
	double approx, exact;
	int a = 0;
	int b = 1000;
	int n = 10000;

	printf("--- montecarlo: sequential integral ---\n");
	approx = montecarlo(a, b, n);
	exact  = (-cos(b)) - (-cos(a));
	printf("montecarlo approx: %.5f\n", approx);
	printf("exact: %.5f\n",  exact);
	return 0;
}


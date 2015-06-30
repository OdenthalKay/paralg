/*
* MIDPOINT
*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

static double f(double x) { return sin(x); }

double midpoint(int a, int b, int n) {
	double h = (double)(b - a) / n;
	double approx = 0;

	for (int i = 0; i <= (n / 2); i++) {
		int index = i * 2;
		double x = a + index * h;
		approx += f(x);
	}

	return 2*h*approx;
}

int main(int argc, char const *argv[])
{
	double approx, exact;
	int a = 0;
	int b = 1000;
	int n = 100000000;

	printf("--- midpoint: sequential integral ---\n");
	approx = midpoint(a, b, n);
	exact  = (-cos(b)) - (-cos(a));
	printf("midpoint approx: %.5f\n", approx);
	printf("exact: %.5f\n",  exact);
	return 0;
}

#include <stdio.h>


int sum(int x, int y) {
	return x+y;
}

int main()
{
	int n = 16;
	int p = 4;
	int a[16] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
	int b[16] = {0};
	int i, j, stride;

	// Calculate partial sums (1 sum for each processor)
	for (i=0;i<4;i++) {
		for (j=0;j<4;j++) {
			b[i] += a[i*4+j];
		}
	}
	print_array(0,b,4);

	// Calculate final sum using the binary tree template algorithm
	for (stride=4/2;stride>0;stride/=2) {
		for (i=0;i<stride;i++) {
			b[i] = sum(b[i], b[i+stride]);
		}
	}
	print_array(0,b,4);


	return 0;
}

void print_array(int threadID, int x[], int size) {
	printf("--- Array of thread with ID %d ---\n", threadID);
	int i;
	for (i=0;i<size;i++) {
		printf("%d: %d\n",i,x[i]);
	}
};
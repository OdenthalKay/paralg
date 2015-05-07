
void sequential_prefix(double x[], int start, int blocksize, double y[]) {
	int i;
	y[0] = x[start];
	for (i=0;i<blocksize-1;i++) {
		y[i+1] = y[i] + x[i+start+1];
	}
}

void fill_array(double x[], int size) {
	int i;
	for (i=0;i<size;i++) {
		x[i] = i;
	}
}

void print_array(int threadID, double x[], int size) {
	printf("--- Array of thread with ID %d ---\n", threadID);
	int i;
	for (i=0;i<size;i++) {
		printf("%d: %f\n",i,x[i]);
	}
};
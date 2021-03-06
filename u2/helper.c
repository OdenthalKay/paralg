
/*
application data type for both a2 and a3
*/
typedef double atype_t;
atype_t sum(atype_t x, atype_t y) {
	return x+y;
}


/*
Used in a3
*/
void sequential_prefix(atype_t x[], int start, int blocksize, atype_t y[]) {
	int i;
	y[0] = x[start];
	for (i=0;i<blocksize-1;i++) {
		y[i+1] = y[i] + x[i+start+1];
	}
}

//////////////////////////////////////////////////////////////////////////

void fill_array(atype_t x[], int size) {
	int i;
	for (i=0;i<size;i++) {
		x[i] = i+1;
	}
}

void print_array(int threadID, double x[], int size) {
	printf("--- Array of thread with ID %d ---\n", threadID);
	int i;
	for (i=0;i<size;i++) {
		printf("%d: %f\n",i,x[i]);
	}
};
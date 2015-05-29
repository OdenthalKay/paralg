#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "libFHBRS/libFHBRS.h"

int cmpfunc(const void *a, const void *b);
int is_sorted(int x[], int length);

int main(int argc, char** argv) {
  if (argc < 2) {
    printf("Wrong number of arguments. Expecting: <n>\n");
    return -1;
  }
  int n = atoi(argv[1]);
  int i;
  int *values;
  double t0, t1;
  ////////////////////////////////////////////////////////

  t0 = gettime();
  values = malloc(n*sizeof(int));
  for (i=0;i<n;i++) {
    values[i] = n-i;
  }
  qsort(values, n, sizeof(int), cmpfunc);
  t1 = gettime();

  assert(is_sorted(values, n));
  free(values);
  printf("%.5f\n",t1-t0);
  return 0;
}

///////////////////////////////////////////////////////////

/* 
returns negative if b > a 
and positive if a > b 
*/  
int cmpfunc(const void *a, const void *b) { 
    const int *x = (const int *)a; // casting pointer types 
    const int *y = (const int *)b;
    return *x  - *y;  
  } 

  int is_sorted(int x[], int length) {
    int i;
    for (i=0;i<length-1;i++) {
      if (x[i+1] < x[i]) {
        return 0;
      }
    }
    return 1;
  }
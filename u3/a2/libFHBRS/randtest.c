/*==============================================================================
  
   Purpose:    test random functions
   Author:     Rudolf Berrendorf
               Computer Science Department
               Bonn-Rhein-Sieg University
	       53754 Sankt Augustin, Germany
               rudolf.berrendorf@h-brs.de
  
==============================================================================*/

#include <stdio.h>

#include "libFHBRS.h"

//==============================================================================

int
main (int argc, char **argv)
{
  long n = 1000000;
  double a[n];
  void * generator;
#define NHIST 10
  long histogram[NHIST];

  // initialize random number generator
  generator = randp_init(0UL);
  
  // clear histogram
  for(long i=0; i<NHIST; i++)
    histogram[i] = 0;

  // generate random numbers
#pragma omp parallel for
  for (long i = 0; i < n; i++)
    a[i] = randp_standard (generator);

  for (long i = 0; i < n; i++)
    {
      int j = (int)(a[i] * NHIST);
      histogram[j]++;
#if 0
      printf("%15.12f ", a[i]);
      if(i % 8 == 0)
	printf("\n");
#endif
    }

  printf("histogram: \n");
  for(long i=0; i<NHIST; i++)
    printf("%2ld %ld\n", i, histogram[i]);

  return 0;
}

/*============================================================================*
 *                             that's all folks                               *
 *============================================================================*/

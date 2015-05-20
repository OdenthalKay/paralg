/*==============================================================================
  
   Purpose:    uniform random numbers
   Author:     Rudolf Berrendorf
               Computer Science Department
               Bonn-Rhein-Sieg University
	       53754 Sankt Augustin, Germany
               rudolf.berrendorf@h-brs.de
  
==============================================================================*/

#include "libFHBRS.h"

#include <stdlib.h>

// should be in stdlib.h, but isn't
extern void srandom(unsigned int seed);


/*============================================================================*/
// initializes the random number generator

void
rand_init (unsigned long seed)
{
  srandom((unsigned int)seed);
}

/*============================================================================*/
// returns a double random number in the intervall [0,1)

double
rand_standard (void)
{
  return rand () / (RAND_MAX + 1.0);
}

/*============================================================================*/
// returns an integer random number in the intervall [0,n)

int
rand_limit (int n)
{
  return rand_standard () * n;
}

/*============================================================================*
 *                             that's all folks                               *
 *============================================================================*/

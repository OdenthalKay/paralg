/*==============================================================================
  
   Purpose:    test timing functions
   Author:     Rudolf Berrendorf
               Computer Science Department
               Bonn-Rhein-Sieg University of Applied Sciences
	       53754 Sankt Augustin, Germany
               rudolf.berrendorf@h-brs.de
  
==============================================================================*/

#include <stdint.h>
#include <stdio.h>

#include "libFHBRS.h"


/*============================================================================*/

int
main (int argc, char **argv)
{
  double tt1, tt2;
  unsigned long long c1, c2;
  int n = 1000000;


  // time
  tt1 = gettime ();
  tt1 = gettime ();
  for(int i=0; i<n; i++)
    tt2 = gettime ();
  printf ("overhead getting time   : %12.9f seconds\n", (tt2 - tt1) / n);

  tt1 = gettime ();
  while((tt2 = gettime()) == tt1)
    ;
  while((tt1 = gettime()) == tt2)
    ;
  printf ("clock resolution        : %12.9f seconds\n", tt1 - tt2);


  // cycles
  c1 = getcycles();
  c1 = getcycles();
  for(int i=0; i<n; i++)
    c2 = getcycles ();
  printf ("overhead getting cycles : %12lld cycles\n", (c2 - c1) / n);

  c1 = getcycles();
  while((c2 = getcycles()) == c1)
    ;
  while((c1 = getcycles()) == c2)
    ;
  printf ("getcycle resolution     : %12lld cycles\n", c1 - c2);


  return 0;
}


/*============================================================================*
 *                             that's all folks                               *
 *============================================================================*/

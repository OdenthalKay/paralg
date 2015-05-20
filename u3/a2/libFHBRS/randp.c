/*==============================================================================
  
   Purpose          : random number generation with mersenne twister algorithm
   Author           : see below for original author
  
==============================================================================*/

#include <stdlib.h>
#include <pthread.h>

#include "libFHBRS.h"

/* 
   A C-program for MT19937-64 (2004/9/29 version).
   Coded by Takuji Nishimura and Makoto Matsumoto.

   This is a 64-bit version of Mersenne Twister pseudorandom number
   generator.

   Before using, initialize the state by using init_genrand64(seed)  
   or init_by_array64(init_key, key_length).

   Copyright (C) 2004, Makoto Matsumoto and Takuji Nishimura,
   All rights reserved.                          

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:

     1. Redistributions of source code must retain the above copyright
        notice, this list of conditions and the following disclaimer.

     2. Redistributions in binary form must reproduce the above copyright
        notice, this list of conditions and the following disclaimer in the
        documentation and/or other materials provided with the distribution.

     3. The names of its contributors may not be used to endorse or promote 
        products derived from this software without specific prior written 
        permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
   A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER
   OR
   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

   References:
   T. Nishimura, ``Tables of 64-bit Mersenne Twisters''
     ACM Transactions on Modeling and 
     Computer Simulation 10. (2000) 348--357.
   M. Matsumoto and T. Nishimura,
     ``Mersenne Twister: a 623-dimensionally equidistributed
       uniform pseudorandom number generator''
     ACM Transactions on Modeling and 
     Computer Simulation 8. (Jan. 1998) 3--30.

   Any feedback is very welcome.
   http://www.math.hiroshima-u.ac.jp/~m-mat/MT/emt.html
   email: m-mat @ math.sci.hiroshima-u.ac.jp (remove spaces)
*/


//==============================================================================

#include <stdio.h>

//==============================================================================
// constants

#define NN 312
#define MM 156
#define MATRIX_A 0xB5026F5AA96619E9ULL
#define UM 0xFFFFFFFF80000000ULL       /* Most significant 33 bits */
#define LM 0x7FFFFFFFULL	       /* Least significant 31 bits */


//==============================================================================
// types

typedef struct
{
  pthread_mutex_t randp_mutex;
  unsigned long long mt[NN]; // The array for the state vector
  int mti; // position
} random_state_t;


//==============================================================================
/* initializes mt[NN] with a seed */

static random_state_t *
init_genrand64 (unsigned long long seed)
{
  random_state_t *state = malloc(sizeof(*state));
  if(state == NULL)
    return NULL;

  pthread_mutex_init(&state->randp_mutex, NULL);
  state->mt[0] = seed;
  int mti;
  for (mti = 1; mti < NN; mti++)
    state->mt[mti] = (6364136223846793005ULL * (state->mt[mti - 1] ^ (state->mt[mti - 1] >> 62)) + mti);
  state->mti = mti;

  return state;
}


//==============================================================================
/* initialize by an array with array-length */
/* init_key is the array for initializing keys */
/* key_length is its length */

static void *
init_by_array64 (unsigned long long init_key[], unsigned long long key_length)
{
  unsigned long long i, j, k;

  random_state_t *generator;

  generator = init_genrand64 (19650218ULL);
  unsigned long long *mt = generator->mt;

  i = 1;
  j = 0;
  k = (NN > key_length ? NN : key_length);

  for (; k; k--)
    {
      mt[i] = (mt[i] ^ ((mt[i - 1] ^ (mt[i - 1] >> 62)) * 3935559000370003845ULL)) + init_key[j] + j;	/* non linear */
      i++;
      j++;
      if (i >= NN)
	{
	  mt[0] = mt[NN - 1];
	  i = 1;
	}
      if (j >= key_length)
	j = 0;
    }

  for (k = NN - 1; k; k--)
    {
      mt[i] = (mt[i] ^ ((mt[i - 1] ^ (mt[i - 1] >> 62)) * 2862933555777941757ULL)) - i;	/* non linear */
      i++;
      if (i >= NN)
	{
	  mt[0] = mt[NN - 1];
	  i = 1;
	}
    }

  mt[0] = 1ULL << 63;		       /* MSB is 1; assuring non-zero initial array */

  return generator;
}

//==============================================================================
/* generates a random number on [0, 2^64-1]-interval */

static unsigned long long
genrand64_int64 (random_state_t *generator)
{
  int i;
  unsigned long long x;
  static unsigned long long mag01[2] = { 0ULL, MATRIX_A };
  unsigned long long *mt = generator->mt;


  if (generator->mti >= NN)
    {				       /* generate NN words at one time */
      
      /* if init_genrand64() has not been called, */
      /* a default initial seed is used */
      if (generator->mti == NN + 1)
	generator = init_genrand64 (5489ULL);
      
      for (i = 0; i < NN - MM; i++)
	{
	  x = (mt[i] & UM) | (mt[i + 1] & LM);
	  mt[i] = mt[i + MM] ^ (x >> 1) ^ mag01[(int) (x & 1ULL)];
	}
      
      for (; i < NN - 1; i++)
	{
	  x = (mt[i] & UM) | (mt[i + 1] & LM);
	  mt[i] = mt[i + (MM - NN)] ^ (x >> 1) ^ mag01[(int) (x & 1ULL)];
	}
      
      x = (mt[NN - 1] & UM) | (mt[0] & LM);
      mt[NN - 1] = mt[MM - 1] ^ (x >> 1) ^ mag01[(int) (x & 1ULL)];
      generator->mti = 0;
    }

  x = mt[generator->mti++];

  x ^= (x >> 29) & 0x5555555555555555ULL;
  x ^= (x << 17) & 0x71D67FFFEDA60000ULL;
  x ^= (x << 37) & 0xFFF7EEE000000000ULL;
  x ^= (x >> 43);

  return x;
}

//==============================================================================
// interface functions


// initializes the random number generator
void *
randp_init (unsigned long seed)
{
  unsigned long long s = seed;

  return init_by_array64 (&s, 1ULL);
}

// returns a double random number in the intervall [0,1)
double
randp_standard (void * generator)
{
  random_state_t *real_generator = generator;
  
  // lock mutex
  pthread_mutex_lock(&real_generator->randp_mutex);

  double x = (genrand64_int64 (real_generator) >> 11) * (1.0 / 9007199254740992.0);

  // unlock mutex
  pthread_mutex_unlock (&real_generator->randp_mutex);

  return x;
}

// returns a vector of double random numbers in the intervall [0,1)
void
randp_vector (void * generator, int n, double a[n])
{
  random_state_t *real_generator = generator;
  
  // lock mutex
  pthread_mutex_lock(&real_generator->randp_mutex);

  for(int i=0; i<n; i++)
    a[i] = (genrand64_int64 (real_generator) >> 11) * (1.0 / 9007199254740992.0);

  // unlock mutex
  pthread_mutex_unlock (&real_generator->randp_mutex);
}

// returns an integer random number in the intervall [0,n)
unsigned long
randp_limit (void * generator, unsigned long n)
{
  return (unsigned long)(randp_standard (generator) * n);
}


/*============================================================================*
 *                             that's all folks                               *
 *============================================================================*/

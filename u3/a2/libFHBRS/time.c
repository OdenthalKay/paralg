/*==============================================================================
  
   Purpose:    basic timing functions
   Author:     Rudolf Berrendorf
               Computer Science Department
               Bonn-Rhein-Sieg University
	       53754 Sankt Augustin, Germany
               rudolf.berrendorf@h-brs.de
  
==============================================================================*/

#include <sys/time.h>
#include <stddef.h>
#include <stdbool.h>
#include <pthread.h>

#include "libFHBRS.h"

/*============================================================================*/

static pthread_mutex_t time_mutex = PTHREAD_MUTEX_INITIALIZER;
// lock mutex
#define lock()   pthread_mutex_lock (&time_mutex)
#define unlock() pthread_mutex_unlock (&time_mutex);


/*============================================================================*/
/* return floating point time value
   To avoid problems with floating point representation numbers are relative
   to a base value.
*/

double
gettime (void)
{
  static bool first_time = true;
  static unsigned long base_value;
  struct timeval t;

  (void) gettimeofday (&t, NULL);

  // lock mutex
  lock();

  if(first_time)
    {
      first_time = false;

      // get base value (internal unit is microseconds)
      base_value =  t.tv_sec * 1000000UL + t.tv_usec;
    }

  // unlock mutex
  unlock();

  unsigned long current_value = (t.tv_sec * 1000000UL + t.tv_usec) - base_value;
  return current_value * 1e-6;
}

/* Fortran */
double
gettime_ (void)
{
  return gettime ();
}

#if defined(JAVA_INTERFACE)
/* Java version */
JNIEXPORT jdouble JNICALL
Java_Timing_gettime (JNIEnv * env, jobject obj)
{
  struct timeval t;

  (void) gettimeofday (&t, NULL);
  return t.tv_sec + t.tv_usec * 1e-6;
}
#endif


/*============================================================================*/
// return current value of cycle counter

unsigned long long
getcycles (void)
{
  unsigned long long ret = 0;

  do {
    unsigned int __a, __d;
    __asm__ volatile("rdtscp" : "=a" (__a), "=d" (__d));
    (ret) = ((unsigned long long)__a) | (((unsigned long long)__d) << 32);
  } while(0);

  return ret;
}


/*============================================================================*
 *                             that's all folks                               *
 *============================================================================*/

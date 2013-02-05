#include <time.h>
#include <sys/time.h>
#include "Timer.hpp"

/* Globally provided Timer. */
Timer Tick;

#define SecToNSec (1000000000)
#define SecTouSec (1000000)
#define SecToMSec (1000)

Timer::Timer( void ) {
  #ifdef _RT
  clock_gettime( CLOCK_REALTIME, &_T1 );
  clock_gettime( CLOCK_REALTIME, &_T2 );
  #else
  gettimeofday( &_T1, NULL );
  gettimeofday( &_T2, NULL );
  #endif
}

/**
   Tick is an alias for Tock.
   Ha, Ha, Ha.
   @return An unsigned long corresponding to how much
   time has passed since the last Tick. Microseconds normally,
   Nanoseconds if _RT was enabled.
**/
unsigned long Timer::Tick( void ) {
  return this->Tock();
}

/**
   Tock returns the time elapsed since the last Tock.
   @return An unsigned long corresponding to how much time
   has passed since the last Tock. Microseconds normally,
   Nanoseconds if _RT was enabled.
**/
unsigned long Timer::Tock( void ) {
#ifdef _RT
  clock_gettime( CLOCK_REALTIME, &_T2 );
  this->delta = (_T2.tv_nsec - _T1.tv_nsec) + (SecToNSec * (_T2.tv_sec - _T1.tv_sec));
  this->scale = (this->delta / (KeyFrameRate * 1000))
#else
  gettimeofday( &_T2, NULL );
  this->delta = (_T2.tv_usec - _T1.tv_usec) + (SecTouSec * (_T2.tv_sec - _T1.tv_sec));
  this->scale = (this->delta / KeyFrameRate);
#endif
  _T1 = _T2;
  return delta;
}

/**
   Delta returns the time elapsed between the last Tick and the last Tock.
   Does not start a new timer.
   @return Time elapsed in Microseconds, or Nanoseconds if _RT was enabled.
**/
unsigned long Timer::Delta( void ) const {
  return delta;
}


/**
   Scale returns the relative lateness or eagerness of the Timer,
   Relative to a benchmark or Key Frame Rate (The default is 60FPS,
   or 16667 msec.)
   @return A non-zero float that ranges from (0,1) indicating that
   the program is rendering faster than 60FPS, or from the range
   [1,+inf) indicating that the program is rendering slower than
   60FPS.
**/
double Timer::Scale( void ) const {
  return scale;
}

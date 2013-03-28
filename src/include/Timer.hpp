#ifndef __TIMER_HPP
#define __TIMER_HPP

#include <time.h>

class Timer {
  
public:
  Timer();
  unsigned long tick();
  unsigned long tock();
  unsigned long delta() const;
  unsigned long keyFrameRate() const;
  unsigned long keyFrameRate( unsigned long newFrameRate );
  double scale() const;

private:
#ifdef _RT
  struct timespec _t1;
  struct timespec _t2;
#else
  struct timeval _t1;
  struct timeval _t2;
#endif
  unsigned long _delta;
  double _scale;
  unsigned long _keyFrameRate;
};

extern Timer tick;
#define DEFAULT_KEYFRAME_RATE (16667.0)
// Key Frame is every 1/60th sec, 16.67 ms or 16667 usec

#endif

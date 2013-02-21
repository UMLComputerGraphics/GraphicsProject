#ifndef __TIMER_HPP
#define __TIMER_HPP

#include <time.h>

class Timer { 

public:
  Timer();  
  unsigned long Tick();
  unsigned long Tock();
  unsigned long Delta() const;
  double Scale() const;

private:
#ifdef _RT
  struct timespec _T1;
  struct timespec _T2;
#else
  struct timeval _T1;
  struct timeval _T2;
#endif
  unsigned long delta;
  double scale;
};

extern Timer Tick;
#define KeyFrameRate (16667.0)
// Key Frame is every 1/60th sec, 16.67 ms or 16667 usec

#endif

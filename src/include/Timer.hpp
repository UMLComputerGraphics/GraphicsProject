/**
 * @file Timer.hpp
 * @author John Huston
 * @date 2013-03-29
 * @brief Timer class header.
 * @details Header for the Timer class, which provides a global timer object.
 */

#ifndef __TIMER_HPP
#define __TIMER_HPP

#include <time.h>
#include "mat.hpp"
#include "vec.hpp"

class Timer {

public:
  Timer();
  unsigned long tick();
  unsigned long tock();
  unsigned long delta() const;
  float keyFrameRate() const;
  void keyFrameRate( float newFrameRate );
  void setTimeUniform(GLuint uniform);
  double scale() const;
  void foxtrotUniformCharlieKilo();

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
  float _keyFrameRate;
  GLuint _uniform;
};

/**
 * tick is a globally provided timer object,
 * named cutely so we can call tick.tock() and feel clever.
 */
extern Timer tick;

/**
 * DEFAULT_KEYFRAME_RATE is defined to be
 * 1/60th of a second, or 60FPS.
 * This is only the default, and can be changed at runtime.
 * The value is in terms of microseconds:
 * 16667 usec = 16.667 msec = 0.016667 sec
 */
#define DEFAULT_KEYFRAME_RATE (16667.0)

#endif

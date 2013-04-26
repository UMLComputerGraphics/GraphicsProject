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
#include <vector>
#include "mat.hpp"
#include "vec.hpp"
#include "Engine.hpp"

/**
 * Timer class: Acts as a stopwatch that can time the delta
 * between two successive ticks. Can also compute a scale
 * ratio against a benchmark framerate.
 *
 * @author John Huston
 * @date 2013-03-30
 */
class Timer {

public:
  /**
   * Constructor: Nothing special. No Arguments.
   */
  Timer();

  /**
   * Tick is an alias for tock. (Ha, ha, ha.)
   * @return An unsigned long corresponding to how much
   * time has passed since the last tick. Microseconds normally,
   * Nanoseconds if _RT was enabled.
   */
  unsigned long tick();

  /**
   * tock returns the time elapsed since the last tock.
   * @return An unsigned long corresponding to how much
   * time has passed since the last tick. Microseconds normally,
   * Nanoseconds if _RT was enabled.
   */
  unsigned long tock();

  /**
   * delta returns the time elapsed between the last tick and the last tock.
   * Does not start a new timer.
   * @return Time elapsed in Microseconds, or Nanoseconds if _RT was enabled.
   */
  unsigned long delta() const;

  /**
   * Retrieves the keyFrameRate in milliseconds.
   * @return A float representing the Key Frame Rate in milliseconds.
   */
  float keyFrameRate() const;

  /**
   * Sets a new keyFrameRate in milliseconds.
   * @param newFrameRate A float representing the new keyFrameRate.
   */
  void keyFrameRate( float newFrameRate );

  /**
   * Scale returns the relative lateness or eagerness of the Timer,
   * Relative to a benchmark or KeyFrame Rate. (The default is 60FPS,
   * or 16667 msec.)
   * @return A non-zero float that ranges from (0,1) indicating that
   * the program is rendering faster than 60FPS, or from the range [1,inf)
   * indicating that the program is rendering slower than 60FPS.
   */
  double scale() const;

  /**
   * Updates the Timer with a GLuint handle to a uniform float on the GPU.
   * @param uniform The GLuint handle to the time float on the GPU.
   */
  void setTimeUniform( GLuint uniform );

  /**
   * Sends the current time over to the shader.
   */
  void sendTime( void );

private:
#ifdef _RT
  /**
   * A benchmark time to compute our delta against.
   */
  struct timespec _t1;
  /**
   * The most recent stopwatch time.
   */
  struct timespec _t2;
#else
  /**
   * A benchmark time to compute our delta against.
   */
  struct timeval _t1;
  /**
   * The most recent stopwatch time.
   */
  struct timeval _t2;
#endif
  /** Our computed difference from the last tick/tock. **/
  unsigned long _delta;
  /**
   * Our scalar factor for how well we are doing relative
   * to the keyFrameRate.
   */
  double _scale;
  /**
   * A configurable Key Frame Rate that we use as a benchmark for
   * animation speeds. Does not actually impact the REAL framerate,
   * so there should be no real reason to change this from 60FPS.
   */
  float _keyFrameRate;
  /**
   * An OpenGL GLuint handle to a un
   */
  GLuint _uniform;

  /**
   * a list of shaders for which, when they are the engine's current shader, we should send time.
   */
  std::vector<GLuint> _timedUniforms;
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

/** Multiplicative constant to convert from Seconds to nanoSeconds. **/
#define SEC_TO_NSEC (1000000000)
/** Multiplicative constant to convert from Seconds to microSeconds. **/
#define SEC_TO_USEC (1000000)
/** Multiplicative constant to convert from Seconds to milliSeconds. **/
#define SEC_TO_MSEC (1000)

#endif

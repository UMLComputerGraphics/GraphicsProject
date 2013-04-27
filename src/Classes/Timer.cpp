/**
 * @file Timer.cpp
 * @author John Huston
 * @date 2013-03-28
 * @brief Implementation for global Timer object.
 * @details Timer provides a class for measuring the time
 * delta between frame renders, and provides fractional
 * scalar values for use with animation speeds to
 * accelerate or slow animations in accordance with
 * the key frame rate.
 */

#include <time.h>
#include <sys/time.h>
#include "Timer.hpp"

/**
 * Globally provided timer object.
 */
Timer tick;

/**
 * Constructor: Nothing special. No Arguments.
 */
Timer::Timer( void ) :
    _delta( 0 ), _scale( 0.0 ), _uniform( 0 ) {
#ifdef _RT
  clock_gettime( CLOCK_REALTIME, &_t1 );
  clock_gettime( CLOCK_REALTIME, &_t2 );
#else
  gettimeofday( &_t1, NULL );
  gettimeofday( &_t2, NULL );
#endif
  
  _keyFrameRate = (DEFAULT_KEYFRAME_RATE);
}

/**
 * Tick is an alias for tock. (Ha, ha, ha.)
 * @return An unsigned long corresponding to how much
 * time has passed since the last tick. Microseconds normally,
 * Nanoseconds if _RT was enabled.
 */
unsigned long Timer::tick( void ) {
  return tock();
}

/**
 * tock returns the time elapsed since the last tock.
 * @return An unsigned long corresponding to how much
 * time has passed since the last tick. Microseconds normally,
 * Nanoseconds if _RT was enabled.
 */
unsigned long Timer::tock( void ) {
#ifdef _RT
  clock_gettime( CLOCK_REALTIME, &_t2 );
  _delta = (_t2.tv_nsec - _t1.tv_nsec) + (SEC_TO_NSEC * (_t2.tv_sec - _t1.tv_sec));
  _scale = (_delta / (keyFrameRate() * 1000))
#else
  gettimeofday( &_t2, NULL );
  _delta = (_t2.tv_usec - _t1.tv_usec)
           + (SEC_TO_USEC * (_t2.tv_sec - _t1.tv_sec));
  _scale = (_delta / keyFrameRate());
#endif
  _t1 = _t2;
  return _delta;
}

/**
 * delta returns the time elapsed between the last tick and the last tock.
 * Does not start a new timer.
 * @return Time elapsed in Microseconds, or Nanoseconds if _RT was enabled.
 */
unsigned long Timer::delta( void ) const {
  return _delta;
}

/**
 * Retrieves the keyFrameRate in milliseconds.
 * @return A float representing the Key Frame Rate in milliseconds.
 */
float Timer::keyFrameRate() const {
  return _keyFrameRate;
}

/**
 * Sets a new keyFrameRate in milliseconds.
 * @param newFrameRate A float representing the new keyFrameRate.
 */
void Timer::keyFrameRate( float newFrameRate ) {
  _keyFrameRate = newFrameRate;
}

/**
 * Scale returns the relative lateness or eagerness of the Timer,
 * Relative to a benchmark or KeyFrame Rate. (The default is 60FPS,
 * or 16667 msec.)
 * @return A non-zero float that ranges from (0,1) indicating that
 * the program is rendering faster than 60FPS, or from the range [1,inf)
 * indicating that the program is rendering slower than 60FPS.
 */
double Timer::scale( void ) const {
  return _scale;
}

/**
 * Updates the Timer with a GLuint handle to a uniform float on the GPU.
 * @param uniform The GLuint handle to the time float on the GPU.
 */
void Timer::setTimeUniform( GLuint uniform ) {
  this->_timedUniforms.push_back(uniform);
}

/**
 * Sends the current time over to the shader.
 */
void Timer::sendTime( void ) {
  if ( _uniform > 0 ) {
    int t = glutGet( GLUT_ELAPSED_TIME );
    for (uint i=0; i < _timedUniforms.size(); i++)
      glUniform1i(_timedUniforms[i], t);

    //couldn't care less
    /*if (glGetError()) {
      fprintf( stderr, "glUniform1i failed in sendTime: glUniform1i( %u, %u )\n",
	       _uniform, t );
    }*/
  }
}

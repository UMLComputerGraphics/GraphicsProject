/**
 * @file globals.h
 * @author John Huston
 * @date 2013-03-13
 * @brief Useful global constants, macros,
 * debugging utilities and preprocessor settings.
 */

#ifndef __GLOBALS_H
#define __GLOBALS_H

#include <cmath>
#include "Util.hpp"

/** A constant factor of conversion for Degrees to Radians. **/
#define DEGREES_TO_RADIANS (M_PI/180)
/** A constant for the square root of 2. **/
#define SQRT2 (1.41421356237)
/** A constant for the square root of 3. **/
#define SQRT3 (1.73205080757)
/** POW5(X) returns X^5. It's quicker than pow(x,5)! **/
#define POW5(X) ((X)*(X)*(X)*(X)*(X))
/** How close can a number be to zero before it should be considered 'zero'? **/
const float DIVIDE_BY_ZERO_TOLERANCE = float( 1.0e-07 );

// Turn on debugging if it's been requested of us by the Makefile environment.
/**
 * DEBUG Controls whether or not extra DEBUG messages are printed out.
 */
#ifndef DEBUG
#define DEBUG false
#define PRINT_LEVEL PRINT_INFO
#else
#undef DEBUG
#define DEBUG true
#define PRINT_LEVEL PRINT_DEBUG
#endif


#ifdef PRINTLEVEL
#undef PRINT_LEVEL
#define PRINT_LEVEL (PRINTLEVEL)
#endif

#ifdef PREMULT
/** @var POSTMULT defines if we are, or are not using a Post-Multiplication system. **/
static const bool POSTMULT = false;
/** @var PREMULT defines if we are, or are not using a Pre-Multiplication system. **/
static const bool PREMULT = true;
#else
// Doxygen gets sad if we don't document this in both places... without the var tag.
/** defines if we are, or are not using a Post-Multiplication system. **/
static const bool POSTMULT = true;
/** defines if we are, or are not using a Pre-Multiplication system. **/
static const bool PREMULT = false;
#endif

/**
 * DEBUG_MOTION controls some additional debug messages
 * for acceleration/velocity.
 */
#ifndef DEBUG_MOTION
#define DEBUG_MOTION (false)
#else
#undef DEBUG_MOTION
#define DEBUG_MOTION (true)
#endif

#endif

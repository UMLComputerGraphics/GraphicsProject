/**
 * @file Util.hpp
 * @author Eric McCann
 * @date 2013-03-29
 * @brief String utilities.
 * @details String and path loading helper functions.
 */

#ifndef __UTIL_HPP_
#define __UTIL_HPP_

#include <string>
#include "OpenGL.h"

namespace Util {

  void InitRelativePaths( int argc, char** argv );

  std::string getRelativePath( const char *path );

  /**
   * Return a randomized float value between [0,1].
   * @return A random float, just for you!
   * @see also: jitter
   */
  float randFloat( void );

  /**
   * Returns a random float between [-H,H].
   * @param H The range for the random float.
   * @return a random float between [-H,H].
   */
  double jitter( double H );



}

typedef enum DEBUG_PRINT_LEVELS {
  PRINT_NONE,
  PRINT_ERROR,
  PRINT_WARNING,
  PRINT_INFO,
  PRINT_DEBUG,
  PRINT_VERBOSE
} DebugPrintLevels;

void gprint( DebugPrintLevels level, const char *format, ...);

GLenum glSanityCheck();
  
#endif /* __UTIL_HPP_ */

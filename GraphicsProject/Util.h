/**
 * @file Util.hpp
 * @author Eric McCann
 * @date 2013-03-29
 * @brief String utilities.
 * @details String and path loading helper functions.
 */

#ifndef ERIC_RULES_HPP_
#define ERIC_RULES_HPP_

namespace Util {

  void InitRelativePaths( int argc, char** argv );

  const char *getRelativePath( const char *path );

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

#endif /* ERIC_RULES_HPP_ */

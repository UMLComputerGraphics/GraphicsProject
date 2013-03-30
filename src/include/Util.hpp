/**
 * @file Util.hpp
 * @author Eric McCann
 * @date 2013-03-29
 * @brief String utilities.
 * @details String and path loading helper functions.
 */

#ifndef ERIC_RULES_HPP_
#define ERIC_RULES_HPP_

namespace VooDoo
{
  void InitRelativePaths( int argc, char** argv );
  const char *getRelativePath(const char *path );
}

#endif /* ERIC_RULES_HPP_ */

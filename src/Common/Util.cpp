/**
 * @file Util.cpp
 * @author Eric McCann
 * @date 2013-03-29
 * @brief String utilities.
 * @details Formerly named "eric_rules.cpp", but he had to know that wouldn't last too long.
 */

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "Util.hpp"

// http://stackoverflow.com/questions/236129/splitting-a-string-in-c
std::vector< std::string > &split( const std::string &s, char delim,
                                   std::vector< std::string > &elems ) {
  std::stringstream ss( s );
  std::string item;
  while ( std::getline( ss, item, delim ) ) {
    elems.push_back( item );
  }
  return elems;
}
std::vector< std::string > split( const std::string &s, char delim ) {
  std::vector< std::string > elems;
  return split( s, delim, elems );
}
bool dirExists(const std::string& where, const std::string& dir) {
  static struct stat s;
  std::stringstream str;
  str << where << dir;
  printf("Checking for existence of %s\n",str.str().c_str());
  int err = stat(str.str().c_str(), &s);
  if(-1 == err) {
    return false;
  } else {
    if(S_ISDIR(s.st_mode)) {
        return true;
    } else {
        return false;
    }
  }
}
namespace Util {
  static std::string dondeestalosshaders;
  /**
   * InitRelativePaths is a preparation step allowing files referred to relative to the executable to be referred to relative to the working dir AND the executable
   *
   * @param argc from main
   * @param argv from main
   *
   * @return NOTHING
   */
  void InitRelativePaths( int argc, char** argv ) {
    char *binloc = argv[0];

    std::vector< std::string > slapchop = split( std::string( binloc ), '/' );

    //slapchop[0..length-2] == path without executable name
    if ( slapchop.size() > 1 ) {
      for (size_t last = slapchop.size() - 1; last >= 0; last=last-1)
      {       
	printf("last = %d\n", last);
        std::stringstream cat;
        for ( size_t i = 0; i < last; i++ )
          cat << slapchop[i] << "/";
        if (dirExists(cat.str(), "shaders"))
	{          
          dondeestalosshaders = cat.str();
          break;
        }
      }
    }
  }

  /**
   * Turns a binary-relative relative path into a working directory relative relative path.
   *
   * @praram the path
   *
   * @return the real path
   */
  std::string getRelativePath( const char *path ) {
    std::stringstream wholepath;

    wholepath << dondeestalosshaders << path;
    printf("Looking for \"%s\"\tExecutable is in \"%s\"\tResolving to \"%s\"\n", path, dondeestalosshaders.c_str(), wholepath.str().c_str());
    return wholepath.str();
  }

/**
 * Return a randomized float value between [0,1].
 * @return A random float, just for you!
 * @see also: jitter
 */
float randFloat( void ) {
  return rand() / (float) RAND_MAX;
}

/**
 * Returns a random float between [-H,H].
 * @param H The range for the random float.
 * @return a random float between [-H,H].
 */
double jitter( double H ) {
  return (-H) + rand() * (H - (-H)) / RAND_MAX;
}

}

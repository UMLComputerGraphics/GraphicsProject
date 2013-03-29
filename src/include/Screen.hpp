/**
 * @file Screen.hpp
 * @author John Huston
 * @date 2013-03-29
 * @brief Screen class header file.
 * @details @see Screen.
 */
#ifndef __SCREEN_HPP
#define __SCREEN_HPP

#include "vec.hpp"
#include "Cameras.hpp"
using Angel::vec2;

class Screen {
  
public:
  
  Screen( int x = 0, int y = 0 );
  Screen( const vec2 &newSize );

  // Set the size
  void size( int x, int y );
  void size( const vec2 &newSize );

  const vec2 &size( void );
  int width( void );
  int height( void );

  const vec2 &center( void );
  int midpointX( void );
  int midpointY( void );

  // Public. Users should be able to interface with the Cameras.
  Cameras _camList;

private:
  
  vec2 _size;
  vec2 _center;
  
};

#endif

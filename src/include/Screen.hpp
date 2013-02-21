#ifndef __SCREEN_HPP
#define __SCREEN_HPP

#include "vec.hpp"
#include "Cameras.hpp"
using Angel::vec2;

class Screen {

public:

  Screen( int x, int y );
  Screen( const vec2 &newSize );

  // Set the size
  void Size( int x, int y );
  void Size( const vec2 &newSize );

  const vec2 &Size( void );
  int Width( void );
  int Height( void );
		
  const vec2 &Center( void );
  int MidpointX( void );
  int MidpointY( void );
		
  // Public. Users should be able to interface with the Cameras.
  Cameras camList;
	
private:
	
  vec2 size;
  vec2 center;
	
};

#endif
